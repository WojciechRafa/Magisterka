//
// Created by wpr on 15.11.24.
//

#include "Detected_objects_displayer.hpp"

#include <utility>


Detected_objects_displayer::Detected_objects_displayer(std::pair<Axes, Axes> axes_, cv::Vec2d area_size_, cv::Vec2d zero_point_,
                                                       Objects_tracker *objects_tracker_ptr_,
                                                       Small_window *small_window_ptr_):
                                                       axes(std::move(axes_)),
                                                       area_size(area_size_),
                                                       zero_point(zero_point_),
                                                       objects_tracker_ptr(objects_tracker_ptr_),
                                                       small_window_ptr(small_window_ptr_),
                                                       reference_lines(sf::Lines)
                                                       {
    if(small_window_ptr != nullptr)
        small_window_ptr->set_additional_graphic(&all_graphics);

    create_grid();
                                                       }

void Detected_objects_displayer::update() {
    if(Configs::Big_windows_parameters::is_displayed_verified_only){
        update_verified_object();
    } else{
        update_triangulated_object();
    }

    all_graphics.clear();

    all_graphics.push_back(std::make_unique<sf::VertexArray>(reference_lines));

    for(auto& detection_graphic: detection_graphics){
        all_graphics.push_back(std::move(detection_graphic));
    }

    last_update_time = clock.getElapsedTime();
}

void Detected_objects_displayer::update_triangulated_object() {
    if(small_window_ptr == nullptr or objects_tracker_ptr == nullptr)
        return;

    detection_graphics.clear();

    std::pair<int, int> axis_nr = {get_axi_nr(axes.first), get_axi_nr(axes.second)};

    for(auto& objects_in_chosen_moment: objects_tracker_ptr->get_all_triangulated_object()){
        for(auto& object: objects_in_chosen_moment.second){
            cv::Vec3d pos_3d = object.first;
            auto first_dim_len = pos_3d[axis_nr.first];
            auto second_dim_len = pos_3d[axis_nr.second];

            bool is_in_board;
            sf::Vector2f pos = objects_tracker_ptr->get_position_of_detected_object_on_main_window(is_in_board,
                                                                                                   first_dim_len,
                                                                                                   second_dim_len);
            if(not is_in_board)
                continue;

            float size;
            if(Configs::Big_windows_parameters::is_const_size){
                size = Configs::Big_windows_parameters::default_object_size;
            } else{
                size = static_cast<float>(object.second) * Configs::Big_windows_parameters::detection_radius_ratio;
            }

            add_detection_graphic(pos, size, Configs::Big_windows_parameters::detection_default_circle_color);
        }
    }
}

void Detected_objects_displayer::update_verified_object() {
    if(small_window_ptr == nullptr or objects_tracker_ptr == nullptr)
        return;

    detection_graphics.clear();

    std::pair<int, int> axis_nr = {get_axi_nr(axes.first), get_axi_nr(axes.second)};

    for(auto& object: objects_tracker_ptr->get_all_verified_object()){
        cv::Vec3d pos_3d = object.size;
        auto first_dim_len = pos_3d[axis_nr.first];
        auto second_dim_len = pos_3d[axis_nr.second];
        bool is_in_board;
        sf::Vector2f pos = objects_tracker_ptr->get_position_of_detected_object_on_main_window(is_in_board,
                                                                                               first_dim_len,
                                                                                               second_dim_len);
        if(not is_in_board)
            continue;

        float size;
        if(Configs::Big_windows_parameters::is_const_size){
            size = Configs::Big_windows_parameters::default_object_size;
        } else{
            size = static_cast<float>(object.size) * Configs::Big_windows_parameters::detection_radius_ratio;
        }
        add_detection_graphic(pos, size, object.color);
    }
}

void Detected_objects_displayer::add_detection_graphic(sf::Vector2f pos, float size, sf::Color color) {
    auto new_circle = std::make_unique<sf::CircleShape>(size);

    new_circle->setOrigin(new_circle->getRadius(), new_circle->getRadius());
    new_circle->setFillColor(color);
    new_circle->setOutlineColor(sf::Color::Black);
    new_circle->setOutlineThickness(Configs::Big_windows_parameters::detection_outline_thickness);

    new_circle->setPosition(pos);

    detection_graphics.emplace_back(std::move(new_circle));
}

void Detected_objects_displayer::create_grid() {
    if(small_window_ptr == nullptr)
        return;

    sf::FloatRect bounds = small_window_ptr->getGlobalBounds();

    float window_interval_x = bounds.width  / (static_cast<float>(area_size[0]) / Configs::Big_windows_parameters::grid_span);
    float window_interval_y = bounds.height / (static_cast<float>(area_size[1]) / Configs::Big_windows_parameters::grid_span);

    int number_of_line_pair_x = static_cast<int>(0.5 * bounds.width / window_interval_x);
    int number_of_line_pair_y = static_cast<int>(0.5 * bounds.height / window_interval_y);

    sf::Vector2f global_zero_point = bounds.getPosition() + sf::Vector2f(bounds.width / 2, bounds.height / 2);

    for(int i = -number_of_line_pair_x; i <= number_of_line_pair_x; i++){
        sf::Color line_color;
        if(i == 0)
            line_color = Configs::Big_windows_parameters::grid_span_zero_point_color;
        else
            line_color = Configs::Big_windows_parameters::grid_span_default_color;

        float pos_x = window_interval_x * static_cast<float>(i) + global_zero_point.x;

        reference_lines.append(sf::Vertex(sf::Vector2f(pos_x, bounds.top), line_color));
        reference_lines.append(sf::Vertex(sf::Vector2f(pos_x, bounds.top + bounds.height), line_color));
    }

    for(int i = -number_of_line_pair_y; i <= number_of_line_pair_y; i++){
        sf::Color line_color;
        if(i == 0)
            line_color = Configs::Big_windows_parameters::grid_span_zero_point_color;
        else
            line_color = Configs::Big_windows_parameters::grid_span_default_color;

        float pos_y = window_interval_y * static_cast<float>(i) + global_zero_point.y;

        reference_lines.append(sf::Vertex(sf::Vector2f(bounds.left, pos_y), line_color));
        reference_lines.append(sf::Vertex(sf::Vector2f(bounds.left + bounds.width, pos_y), line_color));

    }
}
