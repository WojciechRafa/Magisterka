//
// Created by wpr on 22.08.24.
//
#include <iostream>
#include <string>

#include <cmath>
#include <utility>

#include "Projection_image_calculator.hpp"
#include "../Network/sended_struct.hpp"
#include "../main_functions.hpp"

cv::Vec3d Projection_image_calculator::compute_3D_line(const cv::Mat& intrinsicMatrix, const cv::Point2d& imagePoint) {
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

Projection_image_calculator::Projection_image_calculator(
                                                         Axes axis_a_,
                                                         Axes axis_b_,

                                                         Configs::computers_enum computer_type_,

                                                         sf::Vector2f window_pos_,
                                                         sf::Vector2f window_size_,
                                                         sf::Vector2f zero_point_pos_,

                                                         bool are_rays_from_slave_,
                                                         std::vector<std::tuple<cv::Vec2d, cv::Vec2d, cv::Vec2d>>* received_parameters_,

                                                        Rays_intersection_analyzer* rays_intersection_analyzer_ptr_):
        Rays_source(rays_intersection_analyzer_ptr_, computer_type_),
        axis_a(axis_a_),
        axis_b(axis_b_),
        window_pos(window_pos_),
        output_window_size(window_size_),
        output_zero_point_pos(zero_point_pos_),

        are_rays_from_slave(are_rays_from_slave_),
        received_parameters(received_parameters_){

    auto local_hw = Configs::local_computer;
    auto local_hw_folder_name = Configs::hw_folders_name[local_hw];
    std::string main_folder = "../Hw_params/" + local_hw_folder_name;
    internal_matrix = load_camera_matrix(main_folder + "/Camera_internal_parameters.csv");
    external_matrix = load_camera_matrix(main_folder + "/Camera_external_parameters.csv");

    corners_angle[0] = atan2(output_zero_point_pos.y,
                             -output_zero_point_pos.x);

    corners_angle[1] = atan2(output_zero_point_pos.y,
                             -output_zero_point_pos.x + output_window_size.x);

    corners_angle[2] = atan2(output_zero_point_pos.y - output_window_size.y,
                             -output_zero_point_pos.x + output_window_size.x);

    corners_angle[3] = atan2( output_zero_point_pos.y - output_window_size.y,
                             -output_zero_point_pos.x);
}

sf::Vector2f Projection_image_calculator::find_intersection(double axis_a_value, double axis_b_value){
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
Projection_image_calculator::set_additional_drawable_ptr(std::vector<std::unique_ptr<sf::Drawable>>* additional_graphic_) {
    drawable_list = additional_graphic_;
}

std::unique_ptr<sf::RectangleShape>  Projection_image_calculator::get_ray(sf::Vector2f intersection,
                                                                          sf::Color color,
                                                                          float thickness) {
    sf::Vector2f direction = intersection - output_zero_point_pos;

    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    auto line = std::make_unique<sf::RectangleShape>((sf::Vector2f(length, 0.5)));

    line->setPosition(output_zero_point_pos);

    float angle = std::atan2(direction.y, direction.x) * 180.f / M_PI;
    line->setRotation(angle);

    line->setFillColor(color);

    return line;
}

//int Projection_image_calculator::get_axi_nr(Axes axis) {
//    switch (axis) {
//        case Axes::x:
//            return 0;
//        case Axes::y:
//            return 1;
//        case Axes::z:
//            return 2;
//        default:
//            return -1;
//    }
//}


void Projection_image_calculator::update() {
    drawable_list->clear();


    if(parameters != nullptr and not are_rays_from_slave){

        for(int i = 1; i < parameters->numb_labels; i++){ // label have number "0";

            cv::Vec3d box_dir_3D_begin;
            cv::Vec3d box_dir_3D_end;
            cv::Vec3d centroid_dir_3D;

            cv::Point2i box_pos(parameters->stats.at<int>(i, cv::CC_STAT_LEFT),
                                parameters->stats.at<int>(i, cv::CC_STAT_TOP));
            cv::Point2i box_size(parameters->stats.at<int>(i, cv::CC_STAT_WIDTH),
                                 parameters->stats.at<int>(i, cv::CC_STAT_HEIGHT));

            cv::Point2d centroid(parameters->centroids.at<double>(i, 0), parameters->centroids.at<double>(i, 1));

            box_dir_3D_begin = compute_3D_line(internal_matrix, static_cast<cv::Point2d>(box_pos));
            box_dir_3D_end = compute_3D_line(internal_matrix, static_cast<cv::Point2d>(box_pos + box_size));
            centroid_dir_3D = compute_3D_line(internal_matrix, centroid);


//            to_send.emplace_back(box_dir_3D_begin, box_dir_3D_end, centroid_dir_3D);

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

                drawable_list->push_back(std::move(begin_ray));
                drawable_list->push_back(std::move(end_ray));
                drawable_list->push_back(std::move(center_ray));

            }
        }

    }else if(received_parameters != nullptr and are_rays_from_slave){
        for(auto& received_parameter : *received_parameters){

            auto box_dir_3D_begin =multiple_internal(std::get<0>(received_parameter));
            auto box_dir_3D_end =  multiple_internal(std::get<1>(received_parameter));
            auto centroid_dir_3D = multiple_internal(std::get<2>(received_parameter));
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

                drawable_list->push_back(std::move(begin_ray));
                drawable_list->push_back(std::move(end_ray));
                drawable_list->push_back(std::move(center_ray));

            }
        }
    }
    for(auto& projection_drawable: *drawable_list){
        sf::Shape* projection_shape_ptr = dynamic_cast<sf::Shape*>(projection_drawable.get());
        projection_shape_ptr->setPosition(projection_shape_ptr->getPosition() + window_pos);
    }

    last_update_time = clock.getElapsedTime();
}

void Projection_image_calculator::set_parameters(std::shared_ptr<Binarization::Binarized_parameters> parameters_) {
    parameters = std::move(parameters_);
}

cv::Vec3d Projection_image_calculator::multiple_internal(cv::Vec2d &image_params) {
    double fx = internal_matrix.at<double>(0, 0);   // f_x
    double fy = internal_matrix.at<double>(1, 1);   // f_y
    double cx = internal_matrix.at<double>(0, 2);   // c_x
    double cy = internal_matrix.at<double>(1, 2);   // c_y

    double X = (image_params[0] - cx) / fx;
    double Y = (image_params[1] - cy) / fy;
    double Z = 1.0;

    return {X, Y, Z};
}
