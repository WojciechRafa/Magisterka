//
// Created by wpr on 22.08.24.
//
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include "Projection_image_calculate.hpp"

cv::Mat Projection_image_calculate::load_camera_matrix(const std::string& filePath) {
    cv::Mat K = cv::Mat::zeros(3, 3, CV_64F);
    std::ifstream file(filePath);

    if (file.is_open()) {
        std::string line;
        int row = 0;
        while (std::getline(file, line) && row < 3) {
            std::stringstream lineStream(line);
            std::string cell;
            int col = 0;
            while (std::getline(lineStream, cell, ',') && col < 3) {
                K.at<double>(row, col) = std::stof(cell);
                auto temporary = std::stof(cell);
                col++;
            }
            row++;
        }
        file.close();
    } else {
        std::cerr << "File  "<< filePath << " can't be open" << filePath << std::endl;
    }
    return K;
}

cv::Vec3d Projection_image_calculate::compute_3D_line(const cv::Mat& intrinsicMatrix, const cv::Point2d& imagePoint) {
    // Invert the intrinsic matrix to go from image coordinates to camera coordinates
    cv::Mat K_inv = intrinsicMatrix.inv();

    // Convert the 2D image point to homogeneous coordinates (x, y, 1)
    cv::Mat imagePointHomogeneous = (cv::Mat_<double>(3, 1) << imagePoint.x, imagePoint.y, 1.0);

    // Compute the direction of the 3D ray in camera coordinates
    cv::Mat rayDirectionMat = K_inv * imagePointHomogeneous;

    // Normalize the direction vector
    cv::Vec3d rayDirection(rayDirectionMat.at<double>(0, 0), rayDirectionMat.at<double>(1, 0), rayDirectionMat.at<double>(2, 0));
    rayDirection = rayDirection / cv::norm(rayDirection);

    return rayDirection;
}

Projection_image_calculate::Projection_image_calculate(Projection_image_calculate::axes axis_a_,
                                                       Projection_image_calculate::axes axis_b_,
                                                       sf::Vector2f window_pos_,
                                                       sf::Vector2f window_size_,
                                                       sf::Vector2f zero_point_pos_,
                                                       int change_time_):
        axis_a(axis_a_),
        axis_b(axis_b_),
        window_pos(window_pos_),
        output_window_size(window_size_),
        output_zero_point_pos(zero_point_pos_),
        Time_Object(change_time_){
    internal_parameters = load_camera_matrix("../Camera_insert_parameters.csv");

    corners_angle[0] = atan2(output_zero_point_pos.y,
                             -output_zero_point_pos.x);

    corners_angle[1] = atan2(output_zero_point_pos.y,
                             -output_zero_point_pos.x + output_window_size.x);

    corners_angle[2] = atan2(output_zero_point_pos.y - output_window_size.y,
                             -output_zero_point_pos.x + output_window_size.x);

    corners_angle[3] = atan2( output_zero_point_pos.y - output_window_size.y,
                             -output_zero_point_pos.x);
}

sf::Vector2f Projection_image_calculate::find_intersection(double axis_a_value, double axis_b_value){
    auto angle = atan2(axis_b_value, axis_a_value); // radians

    sf::Vector2f intersection;

    if(angle <= corners_angle[0] and angle > corners_angle[1]){
         if(axis_a_value != 0){
             double tg = axis_b_value / axis_a_value;
             intersection = sf::Vector2f(- output_zero_point_pos.y / (float) tg, 0);
         }
         else{
             intersection = sf::Vector2f(output_zero_point_pos.y, 0);
         }
    }else if(angle <= corners_angle[1] and angle > corners_angle[2]){
        float relative_a_direction = output_window_size.x - output_zero_point_pos.x;
        double tg = axis_b_value / axis_a_value;
        intersection = sf::Vector2f(output_window_size.x,  (float) tg * relative_a_direction + output_zero_point_pos.y );
    }else if(angle <= corners_angle[2] and angle > corners_angle[3]){
        if(axis_a_value != 0){
            double tg = axis_b_value / axis_a_value;
            intersection = sf::Vector2f(- output_zero_point_pos.y / (float) tg + output_window_size.x, output_window_size.y);
        }
        else{
            intersection = sf::Vector2f(output_zero_point_pos.y, 0);
        }
    }else{
        double tg = axis_b_value / axis_a_value;
        intersection = sf::Vector2f(0, - (float) tg * output_zero_point_pos.x - output_zero_point_pos.y );
    }
    return intersection;
}

void
Projection_image_calculate::set_additional_graphic(std::shared_ptr<std::vector<std::unique_ptr<sf::Shape>>> additional_graphic_) {
    projections = std::move(additional_graphic_);
}

void Projection_image_calculate::set_vectors_list(std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>>* vectors_list_){
    vectors_list = vectors_list_;
}

std::unique_ptr<sf::RectangleShape>  Projection_image_calculate::get_ray(sf::Vector2f intersection,
                                                                         sf::Color color,
                                                                         float thickness) {
    sf::Vector2f direction = intersection - output_zero_point_pos;

    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    auto line = std::make_unique<sf::RectangleShape>((sf::Vector2f(length, thickness)));

    line->setPosition(output_zero_point_pos);

    float angle = std::atan2(direction.y, direction.x) * 180.f / M_PI;
    line->setRotation(angle);

    line->setFillColor(color);

    return line;
}

int Projection_image_calculate::get_axi_nr(Projection_image_calculate::axes axis) {
    switch (axis) {
        case axes::x:
            return 0;
        case axes::y:
            return 1;
        case axes::z:
            return 2;
        default:
            return -1;
    }
}


void Projection_image_calculate::update() {
    projections->clear();
    objets_parameters_list->clear();
    vectors_list->clear();

    if(parameters != nullptr){
//        int number_of_labels = stats->rows;

        for(int i = 1; i < parameters->numb_labels; i++){ // label have number "0";
            cv::Point2i box_pos(parameters->stats.at<int>(i, cv::CC_STAT_LEFT), parameters->stats.at<int>(i, cv::CC_STAT_TOP));
            cv::Point2i box_size(parameters->stats.at<int>(i, cv::CC_STAT_WIDTH), parameters->stats.at<int>(i, cv::CC_STAT_HEIGHT));

            cv::Point2d centroid(parameters->centroids.at<double>(i, 0), parameters->centroids.at<double>(i, 1));

            objets_parameters_list->emplace_back(static_cast<cv::Point2d>(box_pos), static_cast<cv::Point2d>(box_pos + box_size), centroid);

            auto box_dir_3D_begin = compute_3D_line(internal_parameters, static_cast<cv::Point2d>(box_pos));
            auto box_dir_3D_end = compute_3D_line(internal_parameters, static_cast<cv::Point2d>(box_pos + box_size));
            auto centroid_dir_3D = compute_3D_line(internal_parameters, centroid);

            vectors_list->emplace_back(box_dir_3D_begin, box_dir_3D_end, centroid_dir_3D);

            float line_thickness = 0.5;

            if(output_window_size != sf::Vector2f(0, 0) and window_pos != sf::Vector2f(0, 0)) {

                auto begin_ray =
                        get_ray(find_intersection(box_dir_3D_begin(get_axi_nr(axis_a)),
                                                            box_dir_3D_begin(get_axi_nr(axis_b))),
                                           sf::Color::Yellow,
                                           line_thickness);

                auto end_ray =
                        get_ray(find_intersection(box_dir_3D_end(get_axi_nr(axis_a)),
                                                            box_dir_3D_end(get_axi_nr(axis_b))),
                                sf::Color::Blue,
                                line_thickness);

                auto center_ray =
                        get_ray(find_intersection(   centroid_dir_3D(get_axi_nr(axis_a)),
                                                     centroid_dir_3D(get_axi_nr(axis_b))),
                                sf::Color::Green,
                                line_thickness);

                projections->push_back(std::move(begin_ray));
                projections->push_back(std::move(end_ray));
                projections->push_back(std::move(center_ray));

            }
        }
        for(auto& projection: *projections){
            projection->setPosition(projection->getPosition() + window_pos);
        }
        std::cout<<"Ilosc parametrow : " << parameters->numb_labels <<std::endl;
    }

    last_update_time = clock.getElapsedTime().asMicroseconds();
}

void Projection_image_calculate::set_parameters(std::shared_ptr<Binarization::Binarized_parameters> parameters_) {
    parameters = std::move(parameters_);
}

void Projection_image_calculate::set_objets_parameters(
        std::vector<std::tuple<cv::Vec2d, cv::Vec2d, cv::Vec2d>> *objets_parameters_) {
    objets_parameters_list = objets_parameters_;
}

