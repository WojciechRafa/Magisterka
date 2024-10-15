//
// Created by wpr on 29.09.24.
//

#include <map>
#include <cmath>
#include <opencv2/calib3d.hpp>
#include "Rays_intersection_analyzer.hpp"

void get_potential_detected_object(std::vector<std::tuple<cv::Vec2d, cv::Vec2d, double>>& result,
                                   Frame_parameters& objets_begin,
                                   Frame_parameters& objets_end,
                                   sf::Int64 time_diff);

                            void Rays_intersection_analyzer::add_projection(std::unique_ptr<Frame_parameters> params) {
    objets_parameters_list.push_front(std::move(params));
}



void Rays_intersection_analyzer::update() {

    // sort by time
    objets_parameters_list.sort([](const std::unique_ptr<Frame_parameters>& a, const std::unique_ptr<Frame_parameters>& b) {
        return a->time < b->time;
    });

    std::map<Rays_source*, Frame_parameters*> last_object_of_source;

    // Potential_objets is variable used to storage all pairs of rays witch position and size allow to assume that they are related with the objets
//    using Potential_objets = std::vector<std::tuple<cv::Vec3d, cv::Vec3d, double>>;
//    std::map<Rays_source*, std::tuple<Frame_parameters*, Frame_parameters*, std::unique_ptr<Potential_objets>>> rays_pairs;


    // update objets_pairs_of_same_source
    for(auto& actual_object_ptr : objets_parameters_list) {
        if (last_object_of_source.find(actual_object_ptr->source_ptr) == last_object_of_source.end()) { // don't found another ray from tha objets
            last_object_of_source[actual_object_ptr->source_ptr] = actual_object_ptr.get();
        } else {
            sf::Int64 time_begin = actual_object_ptr->time;
            auto last_ray_params_ptr = last_object_of_source[actual_object_ptr->source_ptr];

            if (objets_pairs_of_same_source.find(std::make_pair(actual_object_ptr->source_ptr, time_begin)) ==
                objets_pairs_of_same_source.end()) {
                auto potential_objets = std::make_unique<Potential_objets>();
                get_potential_detected_object(*potential_objets,
                                              *last_object_of_source[actual_object_ptr->source_ptr],
                                              *actual_object_ptr,
                                              actual_object_ptr->time - last_ray_params_ptr->time);
                objets_pairs_of_same_source[std::make_pair(last_ray_params_ptr->source_ptr, time_begin)] =
                        std::make_tuple(last_object_of_source[actual_object_ptr->source_ptr], actual_object_ptr.get(), std::move(potential_objets));

            }
            last_object_of_source[actual_object_ptr->source_ptr] = actual_object_ptr.get();  // update last ray
        }
    }
    std::vector<std::tuple<cv::Vec3d, cv::Vec3d, double>> potential_detected_objets_pair_all;

    for(auto& objet_ptr : objets_parameters_list){
        if(objets_pairs_of_same_source.find(std::make_pair(objet_ptr->source_ptr, objet_ptr->time)) != objets_pairs_of_same_source.end()){
            sf::Int64 time_begin = objet_ptr->time;
            sf::Int64 time_end_first = objet_ptr->time;

            auto last_object_params_ptr = last_object_of_source[objet_ptr->source_ptr];

            auto& first_objets_pair = objets_pairs_of_same_source[std::make_pair(objet_ptr->source_ptr, time_begin)];
            auto& first_objet_potential_detections = *std::get<2>(first_objets_pair);

            for(auto& compared_ray_pair: objets_pairs_of_same_source){
                auto compared_ray_source = compared_ray_pair.first.first;

                bool is_same_source = compared_ray_source == objet_ptr->source_ptr;
                auto actual_tuple = std::make_tuple(objet_ptr->time, objet_ptr->source_ptr, compared_ray_source);
                bool is_in_already_use = std::find(
                        already_checked_intersections.begin(), already_checked_intersections.end(), actual_tuple) !=
                        already_checked_intersections.end();
                if(is_same_source or is_in_already_use)
                    continue;

                sf::Int64 time_end = std::get<1>(compared_ray_pair.second)->time;
                sf::Int64 time_begin_second = std::get<1>(compared_ray_pair.second)->time;
//                auto& second_ray_potential_objet = *std::get<2>(new_rays_pair);

                auto& second_objet_potential_detections = *std::get<2>(first_objets_pair);

                // cut to chosen time
                interpolate_position_vector(first_objet_potential_detections, time_begin, time_end_first,
                                            time_end, false);
                interpolate_position_vector(second_objet_potential_detections, time_begin_second, time_end,
                                            time_begin,true);

                std::vector<std::tuple<cv::Vec3d, cv::Vec3d, double>> potential_detected_objets_pair;

                calculate_double_intersections( potential_detected_objets_pair,
                                                first_objet_potential_detections,
                                                *objet_ptr->source_ptr,
                                                second_objet_potential_detections,
                                                *compared_ray_source);

//                potential_detected_objets_pair_all.push_back(potential_detected_objets_pair);
                potential_detected_objets_pair_all.insert(potential_detected_objets_pair_all.end(),
                                                          std::make_move_iterator(potential_detected_objets_pair.begin()),
                                                          std::make_move_iterator(potential_detected_objets_pair.end()));
            }
        }


    }

}

void Rays_intersection_analyzer::interpolate_position_vector(
        std::vector<std::tuple<cv::Vec2d, cv::Vec2d, double>> &pos_vector,
        sf::Int64 time_begin,
        sf::Int64 time_end,
        sf::Int64 time_interpolated,
        bool is_first_vector_changed) {
    sf::Int64 time_diff = time_end - time_begin;
    double interpolated_ratio = (double)(time_interpolated - time_begin) / (double)time_diff;

    for(auto& pos_pair: pos_vector){
        auto vec_diff = std::get<1>(pos_pair) - std::get<0>(pos_pair);

        cv::Vec2d* edited_vector;
        if(is_first_vector_changed)
            edited_vector = &std::get<0>(pos_pair);
        else
            edited_vector = &std::get<1>(pos_pair);

        *edited_vector =  std::get<0>(pos_pair) + interpolated_ratio * vec_diff;
    }
}

//void Rays_intersection_analyzer::calculate_intersections(std::vector<std::tuple<cv::Vec3d, double, sf::Int64>> & result,
//                                                         std::vector<std::tuple<cv::Vec3d, cv::Vec3d, double>> *first_projections,
//                                                         Rays_source &first_source,
//                                                         std::vector<std::tuple<cv::Vec3d, cv::Vec3d, double>> *second_projections,
//                                                         Rays_source &second_source, sf::Int64 time_begin,
//                                                         sf::Int64 time_end) {
//
//}


//void get_vector_of_double_size(std::vector<double> &result, std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>>& rays) {
//    result = {};
//
//    for(auto& ray: rays){
//        cv::Vec3d size_vec_3d = std::get<1>(ray);
//        std::vector<double> size_std_vector = {size_vec_3d[0], size_vec_3d[1], size_vec_3d[2]};
//
//        auto max_element = std::max_element(size_std_vector.begin(), size_std_vector.end());
//        size_std_vector.erase(max_element);
//        result.push_back(sqrt(size_std_vector[0] + size_std_vector[1]));
//    }
//}

//double geometric_average(cv::Vec2d vec_2D){
//    return std::sqrt(std::pow(vec_2D[0], 2) + std::pow(vec_2D[1], 2));
//                            }

void get_potential_detected_object(std::vector<std::tuple<cv::Vec2d, cv::Vec2d, double>>& result,   // return 2d position and size
                                   Frame_parameters& objets_begin,
                                   Frame_parameters& objets_end,
                                   sf::Int64 time_diff) {

    result = {};

    const double maximal_absolute_size_change = 0.3; // TODO, defined that from file
    const double maximal_relative_size_change = 1; // per second

    const double maximal_size_change = maximal_absolute_size_change + maximal_relative_size_change * time_diff / 1e6;

    // TODO
    const double max_relative_speed = 100;

    // all element should have same source
    cv::Mat inside_matrix = objets_begin.source_ptr->get_inside_matrix();
    double focal_x = inside_matrix.at<double>(0, 0);
    double focal_y = inside_matrix.at<double>(1, 1);
    double focal_sq_average = std::sqrt(focal_x * focal_x + focal_y + focal_y);

//    std::vector<double> object_begin_sizes;
//    for(auto& object_begin : objets_begin.objets){
//        cv::Vec2d size = object_begin.size;
//        object_begin_sizes.push_back(std::sqrt(std::pow((size[0] * focal_x), 2) + std::pow(size[1] * focal_y, 2)));
//    }
    std::vector<double> object_begin_sizes;
    for(auto& object_begin : objets_begin.objets){
        cv::Vec2d size = object_begin.size;
        object_begin_sizes.push_back(std::sqrt(std::pow((size[0]), 2) + std::pow(size[1], 2)));
    }

//    std::vector<double> rays_end_sizes;
//    for(auto& object_end : objets_end.objets){
//        cv::Vec2d size = object_end.size;
//        rays_end_sizes.push_back(std::sqrt(std::pow((size[0] * focal_x), 2) + std::pow(size[1] * focal_y, 2)));
//    }

    std::vector<double> rays_end_sizes;
    for(auto& object_end : objets_end.objets){
        cv::Vec2d size = object_end.size;
        rays_end_sizes.push_back(std::sqrt(std::pow((size[0]), 2) + std::pow(size[1], 2)));
    }

    for(size_t ray_begin_iterator = 0; ray_begin_iterator < objets_begin.objets.size(); ray_begin_iterator++){
        double object_begin_size = object_begin_sizes[ray_begin_iterator];
        cv::Vec2d object_begin_pos = objets_begin.objets[ray_begin_iterator].position;

        for(size_t ray_end_iterator = 0; ray_end_iterator < objets_end.objets.size(); ray_end_iterator++){
            double ray_end_size = rays_end_sizes[ray_end_iterator];
            cv::Vec2d ray_end_pos = objets_end.objets[ray_end_iterator].position;

            double size_relation = object_begin_size / ray_end_size;
            bool is_size_difference_acceptable = size_relation < maximal_size_change and
                                                 size_relation > 1/maximal_size_change;

            if(not is_size_difference_acceptable)
                continue;

            cv::Vec2d position_diff = object_begin_pos * object_begin_size - ray_end_pos * ray_end_size;
            double position_speed = cv::norm(position_diff) / (double) (time_diff / 1000000) * focal_sq_average;

            if(position_speed < max_relative_speed){
                result.emplace_back(object_begin_pos, ray_end_pos, (object_begin_size + ray_end_size) / 2);
            }
        }
    }
}

//void calculate_intersections(std::vector<std::tuple<cv::Vec2d, double, sf::Int64>>&, // position, size, time
//                             std::vector<std::tuple<cv::Vec2d, cv::Vec2d, double>>& first_projections,
//                             Rays_source& first_source,
//                             std::vector<std::tuple<cv::Vec2d, cv::Vec2d, double>>& second_projections,
//                             Rays_source& second_source,
//                             sf::Int64 time_begin, sf::Int64 time_end){
//
//
//
//                            }

void Rays_intersection_analyzer::calculate_double_intersections(std::vector<std::tuple<cv::Vec3d, cv::Vec3d, double>> &result,
                                                                std::vector<std::tuple<cv::Vec2d, cv::Vec2d, double>>& first_projections,
                                                                Rays_source &first_source,
                                                                std::vector<std::tuple<cv::Vec2d, cv::Vec2d, double>>& second_projections,
                                                                Rays_source &second_source) {
    // get main parameters

//    cv::Mat camera_matrix_first = first_source.get_inside_matrix() * first_source.get_outside_matrix();
//    cv::Mat camera_matrix_second = second_source.get_inside_matrix() * second_source.get_outside_matrix();

//    double focal_x_first = first_source.get_inside_matrix().at<double>(0, 0);
//    double focal_y_first = first_source.get_inside_matrix().at<double>(1, 1);
//    double focal_sq_average_first = std::sqrt(focal_x_first * focal_x_first + focal_y_first + focal_y_first);
//
//    double focal_x_second = second_source.get_inside_matrix().at<double>(0, 0);
//    double focal_y_second = second_source.get_inside_matrix().at<double>(1, 1);
//    double focal_sq_average_second = std::sqrt(focal_x_second * focal_x_second + focal_y_second + focal_y_second);

    cv::Vec2d camera_pos_first(first_source.get_outside_matrix().at<double>(0, 3),
                               first_source.get_outside_matrix().at<double>(1, 3));
    cv::Vec2d camera_pos_second(second_source.get_outside_matrix().at<double>(0, 3),
                                second_source.get_outside_matrix().at<double>(1, 3));

    result = {};

    if(first_projections.size() != second_projections.size())
        throw std::runtime_error("first_projections->size() != second_projections->size()");

    std::vector<cv::Vec2d> points_2d_pos_first_begin, points_2d_pos_second_begin;
    std::vector<double> points_2d_size_first, points_2d_size_second;


    for(size_t i = 0; i < first_projections.size(); i++){
        points_2d_pos_first_begin.emplace_back(std::get<0>((first_projections)[i]));
        points_2d_pos_second_begin.emplace_back(std::get<0>((second_projections)[i]));

        points_2d_size_first.emplace_back(std::get<2>((first_projections)[i]));
        points_2d_size_second.emplace_back(std::get<2>((first_projections)[i]));
    }

    std::vector<cv::Vec3d> pos_3d_begin;
    std::vector<double> size_3d_begin;
    std::vector<bool> is_correct_3D_begin;

    calculate_single_intersections(pos_3d_begin,
                                   size_3d_begin,
                                   is_correct_3D_begin,
                                   points_2d_pos_first_begin,
                                   points_2d_size_first,
                                   first_source,
                                   points_2d_pos_second_begin,
                                   points_2d_size_second,
                                   second_source);


//    std::vector<cv::Point2d> triangulation_points_first_end,   triangulation_points_second_end;
    std::vector<cv::Vec2d> points_2d_pos_first_end, points_2d_pos_second_end;

    for(size_t i = 0; i < first_projections.size(); i++){
//        triangulation_points_first_end.emplace_back(std::get<1>((*first_projections)[i]));
//        triangulation_points_second_end.emplace_back(std::get<1>((*second_projections)[i]));
        points_2d_pos_first_end.emplace_back(std::get<1>((first_projections)[i]));
        points_2d_pos_second_end.emplace_back(std::get<1>((second_projections)[i]));
    }

    std::vector<cv::Vec3d> pos_3d_end;
    std::vector<double> size_3d_end;
    std::vector<bool> is_correct_3D_end;

    calculate_single_intersections(pos_3d_end,
                                   size_3d_end,
                                   is_correct_3D_end,
                                   points_2d_pos_first_end,
                                   points_2d_size_first,
                                   first_source,
                                   points_2d_pos_second_end,
                                   points_2d_size_second,
                                   second_source);


//    cv::Mat points_4D_begin;
//    cv::triangulatePoints(camera_matrix_first,
//                          camera_matrix_second,
//                          points_2d_pos_first_begin,
//                          points_2d_pos_second_begin,
//                          points_4D);
//
//    for (int i = 0; i < points_4D.cols; i++) {
//        cv::Mat x = points_4D.col(i);
//        x /= x.at<float>(3);
//
//        cv::Vec3d detected_object_pos(x.at<float>(0), x.at<float>(1), x.at<float>(2));
////        std::cout << "Punkt 3D: (" << x.at<float>(0) << ", " << x.at<float>(1) << ", " << x.at<float>(2) << ")" << std::endl;
//
//
//    }
    if(is_correct_3D_begin.size() != is_correct_3D_end.size())
        throw std::runtime_error("is_correct_3D_begin.size() != is_correct_3D_end.size()");

    for(size_t i = 0; i < is_correct_3D_begin.size(); i++){
        if(is_correct_3D_begin[i] and is_correct_3D_end[i]){
            // check size relation
            double max_3d_size_relation = 2; // TODO
            double size_relation = size_3d_begin[i] / size_3d_end[i];
            if(size_relation < max_3d_size_relation and size_relation > 1 / max_3d_size_relation){
                double size_av = (size_3d_begin[i] + size_3d_end[i]);
                result.emplace_back(pos_3d_begin[i], pos_3d_end[i], size_av);
            }
        }

    }
}

void Rays_intersection_analyzer::calculate_single_intersections(std::vector<cv::Vec3d> &result_pos,
                                                                std::vector<double> &result_size,
                                                                std::vector<bool> &result_is_correct,
                                                                std::vector<cv::Vec2d> &first_2d_pos,
                                                                std::vector<double> &first_2d_size,
                                                                Rays_source &first_source,
                                                                std::vector<cv::Vec2d> &second_2d_pos,
                                                                std::vector<double> &second_2d_size,
                                                                Rays_source &second_source) {
    cv::Mat camera_matrix_first = first_source.get_inside_matrix() * first_source.get_outside_matrix();
    cv::Mat camera_matrix_second = second_source.get_inside_matrix() * second_source.get_outside_matrix();

    cv::Mat points_4D;
    cv::triangulatePoints(camera_matrix_first,
                          camera_matrix_second,
                          first_2d_pos,
                          second_2d_pos,
                          points_4D);

    double first_focal_x = first_source.get_inside_matrix().at<double>(0, 0);
    double first_focal_y = first_source.get_inside_matrix().at<double>(1, 1);
    double first_focal_sq_average = std::sqrt(first_focal_x * first_focal_x + first_focal_y + first_focal_y);

    double second_focal_x = first_source.get_inside_matrix().at<double>(0, 0);
    double second_focal_y = first_source.get_inside_matrix().at<double>(1, 1);
    double second_focal_sq_average = std::sqrt(second_focal_x * second_focal_x + second_focal_y + second_focal_y);

    auto& first_out_mat = first_source.get_outside_matrix();
    cv::Vec3d first_pos(first_out_mat.at<double>(0, 3),
                        first_out_mat.at<double>(1, 3),
                        first_out_mat.at<double>(2, 3));

    auto& second_out_mat = first_source.get_outside_matrix();
    cv::Vec3d second_pos(second_out_mat.at<double>(0, 3),
                         second_out_mat.at<double>(1, 3),
                         second_out_mat.at<double>(2, 3));

    for (int i = 0; i < points_4D.cols; i++) {
        cv::Mat interpolated_pos_mat = points_4D.col(i);
        interpolated_pos_mat /= interpolated_pos_mat.at<float>(3);

        cv::Vec3d interpolated_pos(interpolated_pos_mat.at<float>(0), interpolated_pos_mat.at<float>(1), interpolated_pos_mat.at<float>(2));

        // position check

        // first:
        cv::Mat point_interpolated_2D_mat_first  = first_source.get_inside_matrix() * first_source.get_outside_matrix() * points_4D;
        cv::Vec2d point_interpolated_2D_first(point_interpolated_2D_mat_first.at<double>(0), point_interpolated_2D_mat_first.at<double>(1));
        double first_2d_error = cv::norm(point_interpolated_2D_first - first_2d_pos[i]);

        double first_distance = cv::norm(interpolated_pos - first_pos);

        double first_3d_error = (first_2d_error / first_focal_sq_average) * sqrt(first_distance);

        // second:
        cv::Mat point_interpolated_2D_mat_second  = second_source.get_inside_matrix() * second_source.get_outside_matrix() * points_4D;
        cv::Vec2d point_interpolated_2D_second(point_interpolated_2D_mat_second.at<double>(0), point_interpolated_2D_mat_second.at<double>(1));
        double second_2d_error = cv::norm(point_interpolated_2D_second - second_2d_pos[i]);

        double second_distance = cv::norm(interpolated_pos - second_pos);

        double second_3d_error = (second_2d_error / second_focal_sq_average) * sqrt(second_distance);

        double max_pos_error = 1;   // TODO
        bool is_pos_ok = first_3d_error < max_pos_error and second_3d_error < max_pos_error;

        // size check
        double first_absolute_size = first_distance * first_2d_size[i] / first_focal_sq_average;
        double second_absolute_size = second_distance * second_2d_size[i] / second_focal_sq_average;
        double size_av = (first_absolute_size + second_absolute_size) / 2;

        double max_size_ratio = 2;   // TODO
        double size_ratio = first_absolute_size / second_absolute_size;
        bool is_size_ratio_ok = size_ratio< max_size_ratio and
                                size_ratio > 1 / max_size_ratio ;

        if(is_pos_ok and is_size_ratio_ok){
            result_pos.push_back(interpolated_pos);
            result_size.emplace_back(size_av);
            result_is_correct.push_back(true);
        }else{
            result_is_correct.push_back(false);
        }
    }
    if(result_is_correct.size() != first_2d_pos.size())
        throw std::runtime_error("result_is_correct.size() != first_2d_pos.size()");
}
