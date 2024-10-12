//
// Created by wpr on 29.09.24.
//

#include <map>
#include <cmath>
#include "Rays_intersection_analyzer.hpp"

void get_potential_detected_object(std::vector<std::tuple<cv::Vec3d, cv::Vec3d, double>>& result,
                                   std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>>& rays_begin,
                                   std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>>& rays_end,
                                   sf::Int64 time_diff);

void calculate_intersections(std::vector<std::tuple<cv::Vec3d, double, sf::Int64>>&, // position, size, time
                            std::vector<std::tuple<cv::Vec3d, cv::Vec3d, double>>* first_projections,
                            Rays_source& first_source,
                            std::vector<std::tuple<cv::Vec3d, cv::Vec3d, double>>* second_projections,
                            Rays_source& second_source,
                            sf::Int64 time_begin, sf::Int64 time_end);


                            void Rays_intersection_analyzer::add_projection(std::unique_ptr<Rays_parameters> params) {
    rays_parameters_list.push_front(std::move(params));
}



void Rays_intersection_analyzer::update() {

    // sort by time
    rays_parameters_list.sort([](const std::unique_ptr<Rays_parameters>& a, const std::unique_ptr<Rays_parameters>& b) {
        return a->time < b->time;
    });

    std::map<Rays_source*, Rays_parameters*> last_ray;

    // Potential_objets is variable used to storage all pairs of rays witch position and size allow to assume that they are related with the objets
//    using Potential_objets = std::vector<std::tuple<cv::Vec3d, cv::Vec3d, double>>;
//    std::map<Rays_source*, std::tuple<Rays_parameters*, Rays_parameters*, std::unique_ptr<Potential_objets>>> rays_pairs;


    // update rays_pairs_of_same_source
    for(auto& ray_ptr : rays_parameters_list) {
        if (last_ray.find(ray_ptr->source_ptr) == last_ray.end()) { // don't found another ray from tha objets
            last_ray[ray_ptr->source_ptr] = ray_ptr.get();
        } else {
            sf::Int64 time_begin = ray_ptr->time;
            auto last_ray_params_ptr = last_ray[ray_ptr->source_ptr];

            if (rays_pairs_of_same_source.find(std::make_pair(ray_ptr->source_ptr, time_begin)) ==
                rays_pairs_of_same_source.end()) {
                auto potential_objets = std::make_unique<Potential_objets>();
                get_potential_detected_object(*potential_objets,
                                              last_ray_params_ptr->rays,
                                              ray_ptr->rays,
                                              ray_ptr->time - last_ray_params_ptr->time);
                rays_pairs_of_same_source[std::make_pair(last_ray_params_ptr->source_ptr, time_begin)] =
                        std::make_tuple(last_ray[ray_ptr->source_ptr], ray_ptr.get(), std::move(potential_objets));

            }
            last_ray[ray_ptr->source_ptr] = ray_ptr.get();  // update last ray
        }
    }

    for(auto& ray_ptr : rays_parameters_list){
        if(rays_pairs_of_same_source.find(std::make_pair(ray_ptr->source_ptr, ray_ptr->time)) != rays_pairs_of_same_source.end()){
            sf::Int64 time_begin = ray_ptr->time;
            auto last_ray_params_ptr = last_ray[ray_ptr->source_ptr];

            auto& first_rays_pair = rays_pairs_of_same_source[std::make_pair(ray_ptr->source_ptr, time_begin)];
            auto& first_ray_potential_objet = *std::get<2>(first_rays_pair)

            for(auto& compared_ray_pair: rays_pairs_of_same_source){
                auto compared_ray_source = compared_ray_pair.first.first;

                bool is_same_source = compared_ray_source == ray_ptr->source_ptr;
                auto actual_tuple = std::make_tuple(ray_ptr->time, ray_ptr->source_ptr, compared_ray_source);
                bool is_in_already_use = std::find(
                        already_checked_intersections.begin(), already_checked_intersections.end(), actual_tuple) !=
                        already_checked_intersections.end();
                if(is_same_source or is_in_already_use)
                    continue;

                sf::Int64 time_end = std::get<1>(compared_ray_pair.second)->time;
                auto& second_ray_potential_objet = *std::get<2>(new_rays_pair)



            }
        }


    }

}

void get_vector_of_double_size(std::vector<double> &result, std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>>& rays) {
    result = {};

    for(auto& ray: rays){
        cv::Vec3d size_vec_3d = std::get<1>(ray);
        std::vector<double> size_std_vector = {size_vec_3d[0], size_vec_3d[1], size_vec_3d[2]};

        auto max_element = std::max_element(size_std_vector.begin(), size_std_vector.end());
        size_std_vector.erase(max_element);
        result.push_back(sqrt(size_std_vector[0] + size_std_vector[1]));
    }
}

void get_potential_detected_object(std::vector<std::tuple<cv::Vec3d, cv::Vec3d, double>>& result,
                                                          std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>>& rays_begin,
                                                          std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>>& rays_end,
                                                          sf::Int64 time_diff) {

    result = {};

    const double maximal_absolute_size_change = 0.3; // TODO, defined that from file
    const double maximal_relative_size_change = 1; // per second

    const double maximal_size_change = maximal_absolute_size_change + maximal_relative_size_change * time_diff / 1e6;

    // TODO
    const double max_relative_speed = 100;

    std::vector<double> rays_begin_sizes;
    get_vector_of_double_size(rays_begin_sizes, rays_begin);

    std::vector<double> rays_end_sizes;
    get_vector_of_double_size(rays_end_sizes, rays_end);

    for(size_t ray_begin_iterator = 0; ray_begin_iterator < rays_begin.size(); ray_begin_iterator++){
        double ray_begin_size = rays_begin_sizes[ray_begin_iterator];
        cv::Vec3d ray_begin_pos = std::get<2>(rays_begin[ray_begin_iterator]);


        for(size_t ray_end_iterator = 0; ray_end_iterator < rays_end.size(); ray_end_iterator++){
            double ray_end_size = rays_end_sizes[ray_end_iterator];
            cv::Vec3d ray_end_pos = std::get<2>(rays_end[ray_end_iterator]);


            bool is_size_difference_acceptable = (ray_begin_size / ray_end_size) < maximal_size_change or
                                                 (ray_end_size / ray_begin_size) < maximal_size_change;

            if(not is_size_difference_acceptable)
                continue;

            cv::Vec3d position_diff = ray_begin_pos * ray_begin_size - ray_end_pos * ray_end_size;
            double position_speed = cv::norm(position_diff) / (double) (time_diff / 1000000);

            if(position_speed < max_relative_speed){
                result.emplace_back(ray_begin_pos, ray_end_pos, (ray_begin_size + ray_end_size) / 2);
            }
        }
    }
}

void calculate_intersections(std::vector<std::tuple<cv::Vec3d, double, sf::Int64>>&, // position, size, time
                             std::vector<std::tuple<cv::Vec3d, cv::Vec3d, double>>& first_projections,
                             Rays_source& first_source,
                             std::vector<std::tuple<cv::Vec3d, cv::Vec3d, double>>& second_projections,
                             Rays_source& second_source,
                             sf::Int64 time_begin, sf::Int64 time_end){
                                


                            }
