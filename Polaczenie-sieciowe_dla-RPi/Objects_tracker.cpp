//
// Created by wpr on 25.10.24.
//

#include "Objects_tracker.hpp"
#include "Configs.hpp"
#include "Image_Analysis/Projection_image_calculator.hpp"

void Objects_tracker::set_small_window(Small_window* small_window_ptr_) {
    small_window_ptr = small_window_ptr_;
    small_window_ptr->set_additional_graphic(&list_of_graphic);
}

void Objects_tracker::update() {
    std::vector<sf::Int64> time_to_remove_list;

    // remove old detection
    for(auto& objet: detected_objets){
        if(clock.getElapsedTime().asMicroseconds() > (objet.first + Configs::Big_window_parameters::displayed_time.asMicroseconds())){
            time_to_remove_list.push_back(objet.first);
        }
    }

    for(sf::Int64 to_remove: time_to_remove_list){
        detected_objets.erase(to_remove);
    }

    if(small_window_ptr != nullptr){
        list_of_graphic.clear();

        for(auto& objets_list: detected_objets){
            for(auto& objet: objets_list.second) {

                auto new_circle = std::make_unique<sf::CircleShape>(static_cast<float>(objet.second) *
                        Configs::Big_window_parameters::circle_radius_relation);
                new_circle->setOrigin(new_circle->getRadius(), new_circle->getRadius());
                new_circle->setFillColor(Configs::Big_window_parameters::circle_color);

                bool is_in_board;
                auto first_dim_len = objet.first[Projection_image_calculator::get_axi_nr(
                        Configs::Big_window_parameters::first_axle)];
                auto second_dim_len = objet.first[Projection_image_calculator::get_axi_nr(
                        Configs::Big_window_parameters::second_axle)];

                sf::Vector2f pos = get_position_of_detected_object_on_main_window(
                        is_in_board, first_dim_len, second_dim_len);

                if(not is_in_board){
                    continue;
                }
                new_circle->setPosition(pos);

                list_of_graphic.emplace_back(std::move(new_circle));
            }
        }
    }

    last_update_time = clock.getElapsedTime();
}

void Objects_tracker::add_detecion(sf::Int64 time, std::vector<cv::Vec3d>& pos_vector, std::vector<double>& size_vector) {
    if(pos_vector.size() != size_vector.size()){
        throw std::runtime_error("pos_vector.size() != size_vector.size()");
    }

    for(size_t i = 0; i < pos_vector.size(); i++){
        detected_objets[time].emplace_back(pos_vector[i], size_vector[i]);
    }
}

std::map<sf::Int64, std::vector<std::pair<cv::Vec3d, double>>>&
Objects_tracker::get_actual_objets_list() {
    return detected_objets;
}

sf::Vector2f
Objects_tracker::get_position_of_detected_object_on_main_window(bool& is_in_board, double first_dimension, double second_dimension) {
    first_dimension += Configs::Big_window_parameters::zero_point_meters[0];
    second_dimension += Configs::Big_window_parameters::zero_point_meters[1];

    is_in_board = first_dimension < Configs::Big_window_parameters::size_meters[0] and
                  second_dimension < Configs::Big_window_parameters::size_meters[1] and
                  first_dimension > 0 and second_dimension > 0;

    if(not is_in_board or small_window_ptr == nullptr){
        return {-1, -1};
    }

    sf::Vector2f result = sf::Vector2f(static_cast<float>(first_dimension / Configs::Big_window_parameters::size_meters[0]),
                                       static_cast<float>(second_dimension / Configs::Big_window_parameters::size_meters[1]));

    result += small_window_ptr->getPosition();
    return result;
}
