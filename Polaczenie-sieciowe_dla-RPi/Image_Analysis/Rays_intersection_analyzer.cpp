//
// Created by wpr on 29.09.24.
//

#include <map>
#include <opencv2/calib3d.hpp>
#include "Rays_intersection_analyzer.hpp"
#include "../Rays_source.hpp"
#include "../main_functions.hpp"

Rays_intersection_analyzer::Rays_intersection_analyzer() = default;

void get_potential_detected_object(std::vector<std::tuple<cv::Vec2d, cv::Vec2d, double>>& result,
                                   Frame_parameters& objets_begin,
                                   Frame_parameters& objets_end,
                                   sf::Int64 time_diff);

void Rays_intersection_analyzer::add_projection(const std::shared_ptr<Frame_parameters>& new_params) {
    for(auto& parameters_in_time: objets_parameters_list_by_time[new_params->time]){
        if(parameters_in_time->source_ptr == new_params->source_ptr){
            std::cout<<"Parameters sent twice, ptr "<<new_params->source_ptr<<" time : "<< new_params->time.asMicroseconds() << " us\n";
            return;
        }
    }
    objets_parameters_list_by_time[new_params->time].push_back(new_params);
    last_update_time_of_ray_source[new_params->source_ptr] = new_params->time;
}



void Rays_intersection_analyzer::update() {

    for (auto &time_and_parameters_ptr: objets_parameters_list_by_time) {
        auto &list_of_parameters = time_and_parameters_ptr.second;

        for (auto &first_parameter: list_of_parameters) {
            for (auto &second_parameter: list_of_parameters) {
                auto checked_tuple = std::make_tuple(time_and_parameters_ptr.first, first_parameter->source_ptr,
                                                     second_parameter->source_ptr);
                auto checked_tuple_reverse = std::make_tuple(time_and_parameters_ptr.first,
                                                             second_parameter->source_ptr, first_parameter->source_ptr);
                if (std::find(already_checked_intersections.begin(), already_checked_intersections.end(),
                              checked_tuple) != already_checked_intersections.end() or
                    std::find(already_checked_intersections.begin(), already_checked_intersections.end(),
                              checked_tuple_reverse) != already_checked_intersections.end()
                        ) {
                    // element was already checked
                    continue;
                }

                std::vector<cv::Vec3d> result_pos;
                std::vector<double> result_size;

                calculate_intersections(result_pos, result_size,
                                        first_parameter.get(), second_parameter.get());

                if(objects_tracker_ptr != nullptr and !result_pos.empty()){
                    objects_tracker_ptr->add_detection(time_and_parameters_ptr.first,
                                                       result_pos,
                                                       result_size);
                }
                already_checked_intersections.push_back(checked_tuple_reverse);
            }
        }

    }
}

void Rays_intersection_analyzer::calculate_intersections(std::vector<cv::Vec3d> &result_pos,
                                                         std::vector<double> &result_size,
                                                         Frame_parameters* first_frame_params,
                                                         Frame_parameters* second_frame_params) {
    result_pos = {};

    auto first_source = first_frame_params->source_ptr;
    auto second_source = second_frame_params->source_ptr;

    cv::Mat camera_matrix_first = first_source->get_internal_matrix() * first_source->get_external_matrix();
    cv::Mat camera_matrix_second = second_source->get_internal_matrix() * second_source->get_external_matrix();

    sf::Time time_sum = sf::seconds(0);

    for(auto& objet_parameter_first: first_frame_params->objets){

        std::vector<cv::Vec2d> first_2d_centroid_vectors = {};
        std::vector<cv::Vec2d> second_2d_centroid_vectors = {};

        std::vector<cv::Vec2d> first_2d_bb_pos_vectors = {};
        std::vector<cv::Vec2d> second_2d_bb_pos_vectors = {};

        std::vector<cv::Vec2d> first_2d_bb_size_vectors = {};
        std::vector<cv::Vec2d> second_2d_bb_size_vectors = {};

        for(auto& objet_parameter_second: second_frame_params->objets){
            first_2d_centroid_vectors.push_back(objet_parameter_first.centroid);
            second_2d_centroid_vectors.push_back(objet_parameter_second.centroid);

            first_2d_bb_pos_vectors.push_back(objet_parameter_first.bb_position);
            second_2d_bb_pos_vectors.push_back(objet_parameter_second.bb_position);

            first_2d_bb_size_vectors.push_back(objet_parameter_first.bb_size);
            second_2d_bb_size_vectors.push_back(objet_parameter_second.bb_size);
        }

        cv::Mat points_4D;
        auto time_begin = clock.getElapsedTime();
        cv::triangulatePoints(camera_matrix_first,
                              camera_matrix_second,
                              first_2d_centroid_vectors,
                              second_2d_centroid_vectors,
                              points_4D);
        auto time_end = clock.getElapsedTime();

        time_sum += (time_end - time_begin);

        for (int i = 0; i < points_4D.cols; i++){
            cv::Mat interpolated_pos_mat = points_4D.col(i);

            bool projection_2d_is_correct_first = check_2d_projection(interpolated_pos_mat,
                                                                      first_source->get_external_matrix(),
                                                                      first_source->get_internal_matrix(),
                                                                      first_2d_bb_pos_vectors[i],
                                                                      first_2d_bb_size_vectors[i]);

            bool projection_2d_is_correct_second = check_2d_projection(interpolated_pos_mat,
                                                                       second_source->get_external_matrix(),
                                                                       second_source->get_internal_matrix(),
                                                                       second_2d_bb_pos_vectors[i],
                                                                       second_2d_bb_size_vectors[i]);
            double estimated_size;
            bool size_comparison_is_correct = check_size_comparison(
                    estimated_size,
                    interpolated_pos_mat,
                    *first_source,
                    first_2d_bb_size_vectors[i],
                    *second_source,
                    second_2d_bb_size_vectors[i]
                    );
            // test can be miss
            bool is_2d_correct_or_miss = not Configs::is_in_2d_field_tested or
                                        (projection_2d_is_correct_first and projection_2d_is_correct_second);

            bool is_3d_size_correct_or_miss = not Configs::is_3d_size_correct_test or
                                              size_comparison_is_correct;

            if(is_2d_correct_or_miss and is_3d_size_correct_or_miss){
                result_pos.emplace_back(interpolated_pos_mat.at<double>(0, 0),
                                        interpolated_pos_mat.at<double>(1, 0),
                                        interpolated_pos_mat.at<double>(2, 0));
                result_size.push_back(estimated_size);
            }

        }
    }
    std::cout<<"Time sum : "<<time_sum.asMilliseconds()<<" ms "<<std::endl;
}

bool Rays_intersection_analyzer::check_2d_projection(const cv::Mat& position,
                                                     const cv::Mat& outside_matrix,
                                                     const cv::Mat& inside_matrix,
                                                     cv::Vec2d bb_pos_2d, cv::Vec2d bb_size_2d) {
    cv::Mat projection_2d_pos_mat = inside_matrix * outside_matrix * position;
    cv::Vec2d projection_2d_pos = cv::Vec2d(projection_2d_pos_mat.at<double>(0, 0),
                                                projection_2d_pos_mat.at<double>(1, 0));

    return  projection_2d_pos[0] > bb_pos_2d[0] and projection_2d_pos[1] > bb_pos_2d[1] and
            projection_2d_pos[0] < bb_size_2d[0] and projection_2d_pos[1] < bb_size_2d[1];
}

bool Rays_intersection_analyzer::check_size_comparison(   double& estimated_size,
                                                                 const cv::Mat &position_mat,
                                                                 Rays_source first_ray_source,
                                                                 const cv::Vec2d& first_bb_size_2d,
                                                                 Rays_source& second_ray_source,
                                                                 const cv::Vec2d& second_bb_size_2d) {

    auto& first_outside_matrix = first_ray_source.get_external_matrix();
    auto& second_outside_matrix = second_ray_source.get_external_matrix();

    cv::Vec3d first_pos(first_outside_matrix.at<double>(0, 3),
                        first_outside_matrix.at<double>(1, 3),
                        first_outside_matrix.at<double>(2, 3));

    cv::Vec3d second_pos(second_outside_matrix.at<double>(0, 3),
                         second_outside_matrix.at<double>(1, 3),
                         second_outside_matrix.at<double>(2, 3));

    cv::Vec3d position(position_mat.at<double>(0, 3),
                       position_mat.at<double>(1, 3),
                       position_mat.at<double>(2, 3));

    double first_distance = cv::norm(position - first_pos);
    double second_distance = cv::norm(position - second_pos);

    double first_size_norm = get_estimated_3d_size(first_bb_size_2d, first_distance, first_ray_source);

    double second_size_norm = get_estimated_3d_size(second_bb_size_2d, second_distance, second_ray_source);

    estimated_size = (first_size_norm + second_size_norm) / 2;

    return is_in_limit(first_size_norm, second_size_norm, Configs::max_intersection_size_comparison);
}

void Rays_intersection_analyzer::set_objects_tracker_ptr(Objects_tracker *objects_tracker_) {
    objects_tracker_ptr = objects_tracker_;
}

double Rays_intersection_analyzer::get_estimated_3d_size(cv::Vec2d size_2d, double distance_3d, Rays_source &rays_source) {
    double focal_x = rays_source.get_internal_matrix().at<double>(0, 0);
    double focal_y = rays_source.get_internal_matrix().at<double>(1, 1);

    // x_o = x_mm_max * x_p / x_p_max
    cv::Vec2d absolute_2d_size;
    absolute_2d_size[0] = rays_source.get_camera_params()->camera_size_mm[0] /
                                (static_cast<double >(rays_source.get_camera_params()->camera_size_pixels[0]) / size_2d[0]);
    absolute_2d_size[1] = rays_source.get_camera_params()->camera_size_mm[1] /
                                (static_cast<double >(rays_source.get_camera_params()->camera_size_pixels[1]) / size_2d[1]);

    // X_c = Z_c * x_o / f
    cv::Vec2d absolute_3d_size;
    absolute_3d_size[0] =  distance_3d * absolute_2d_size[0] / focal_x;
    absolute_3d_size[1] =  distance_3d * absolute_2d_size[1] / focal_y;

    return cv::norm(absolute_3d_size);
}
