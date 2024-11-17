//
// Created by wpr on 25.10.24.
//

#include "Objects_tracker.hpp"
#include "Configs.hpp"
#include "Image_Analysis/Projection_image_calculator.hpp"


void Objects_tracker::update() {
    // remove old detection
    std::vector<sf::Time> time_to_remove_list;
    for(auto& objet: triangulated_objets){
        if(clock.getElapsedTime() > (objet.first + Configs::Big_windows_parameters::displayed_time)){
            time_to_remove_list.push_back(objet.first);
        }
    }
    for(auto to_remove: time_to_remove_list){
        triangulated_objets.erase(to_remove);
    }


    // update Objets
    for(auto& object: verified_object_list){
        //is displayed
        object.is_displayed = object.position_list.size() >= Configs::Object_tracker::min_detect_number;
    }
    verified_object_list.erase(
            std::remove_if(verified_object_list.begin(), verified_object_list.end(),
                           [](const Verified_object& obj) { return (clock.getElapsedTime() - obj.last_update) >
                                                                   Configs::Object_tracker::remove_time;}),
            verified_object_list.end()
    );

    last_update_time = clock.getElapsedTime();
}

void Objects_tracker::add_detection(sf::Time time, std::vector<cv::Vec3d>& pos_vector, std::vector<double>& size_vector) {
    if(pos_vector.size() != size_vector.size()){
        throw std::runtime_error("pos_vector.size() != size_vector.size()");
    }
    std::cout<<"Add projection"<<std::endl;
    for(size_t i = 0; i < pos_vector.size(); i++){
        triangulated_objets[time].emplace_back(pos_vector[i], size_vector[i]);

        // TODO, avoid double adding
        auto closed_object_ptr = get_closed_object(pos_vector[i], size_vector[i]);
        if(closed_object_ptr != nullptr){
            closed_object_ptr->position_list.push_back(pos_vector[i]);
            closed_object_ptr->size = size_vector[i];
            closed_object_ptr->last_update = time;
        }else{
            verified_object_list.emplace_back(
                    std::vector<cv::Vec3d>{pos_vector[i]},
                        size_vector[i],
                        get_random_color(),
                        time
                    );
        }
    }
}

sf::Vector2f
Objects_tracker::get_position_of_detected_object_on_main_window(bool& is_in_board, double first_dimension, double second_dimension) {
    first_dimension *= Configs::Big_windows_parameters::position_ratio;
    second_dimension *= Configs::Big_windows_parameters::position_ratio;

    first_dimension += Configs::Big_windows_parameters::zero_point_meters[0];
    second_dimension += Configs::Big_windows_parameters::zero_point_meters[1];

    is_in_board = first_dimension < Configs::Big_windows_parameters::size_meters[0] and
                  second_dimension < Configs::Big_windows_parameters::size_meters[1] and
                  first_dimension > 0 and second_dimension > 0;

    if(not is_in_board or small_window_ptr == nullptr){
        return {-1, -1};
    }

    sf::Vector2f result = sf::Vector2f(static_cast<float>(first_dimension / Configs::Big_windows_parameters::size_meters[0]),
                                       static_cast<float>(second_dimension / Configs::Big_windows_parameters::size_meters[1]));

    result.x *= small_window_ptr->getSize().x;
    result.y *= small_window_ptr->getSize().y;

    result += small_window_ptr->getPosition();
    return result;
}

Objects_tracker::Verified_object *Objects_tracker::get_closed_object(const cv::Vec3d& position, double size) {
    Objects_tracker::Verified_object * closet_object_ptr = nullptr;
    double min_distance = -1;

    for(auto& object: verified_object_list){
        double size_proportion = object.size / size;

        if(size_proportion > Configs::Object_tracker::max_size_proportion or
           size_proportion < 1 / Configs::Object_tracker::max_size_proportion){
            continue;
        }

        double distance = cv::norm(object.position_list.back() - position);

        if(distance > Configs::Object_tracker::max_distance){
            continue;
        }

        if(closet_object_ptr == nullptr or distance < min_distance){
            closet_object_ptr = &object;
            min_distance = distance;
        }
    }
    return closet_object_ptr;
}

sf::Color Objects_tracker::get_random_color() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    int red = std::rand() % 256;
    int green = std::rand() % 256;
    int blue = std::rand() % 256;

    return sf::Color(red, green, blue);
}

std::map<sf::Time, std::vector<std::pair<cv::Vec3d, double>>>&
Objects_tracker::get_all_triangulated_object() {
    return triangulated_objets;
}

std::vector<Objects_tracker::Verified_object> &Objects_tracker::get_all_verified_object() {
    return verified_object_list;
}
