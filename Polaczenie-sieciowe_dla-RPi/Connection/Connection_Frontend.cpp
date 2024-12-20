//
// Created by wr on 12/17/22.
//

#include "Connection_Frontend.hpp"
#include <iostream>
#include "../GUI/Custom_Data_IO_Window.hpp"
#include "../main_functions.hpp"

Connection_Frontend::Connection_Frontend(
                                        std::unique_ptr<Buttons_Field> buttons_field_,

                                         sf::Vector2f custom_data_view_pos,
                                         sf::Vector2f custom_data_view_size,
                                         const unsigned int character_size,
                                         sf::Color custom_data_background_color,
                                        std::vector<Custom_Data_IO_Window::message>& message_list,

                                         sf::Vector2f camera_view_pos,
                                         sf::Vector2f camera_view_size,
                                         Graphic_Warehouse& graphic_warehouse):
        buttons_field(std::move(buttons_field_)),
        camera_view(std::make_unique<sf::RectangleShape>(camera_view_size)),
        camera_view_target_size(camera_view_size),
        texture(graphic_warehouse.get_texture("Camera_view_deafult")),

        custom_data_io_window(
                std::make_unique<Custom_Data_IO_Window>(
                custom_data_view_pos,
                custom_data_view_size,
                character_size,
                custom_data_background_color
                              ))
                              {

        camera_view->setPosition(camera_view_pos);
        camera_view->setTexture(&texture);
        camera_view->setSize(static_cast<sf::Vector2f>(texture.getSize()));
        resize_shape(static_cast<sf::Vector2u>(camera_view_target_size), camera_view.get());

        for (auto &message: message_list) {
            custom_data_io_window->add_message(message);
        }

}


[[maybe_unused]] Connection_Frontend::Connection_Frontend(std::unique_ptr<Buttons_Field> buttons_field_,
                                         sf::Vector2f custom_data_view_pos,
                                         sf::Vector2f custom_data_view_size,
                                         unsigned int character_size,
                                         sf::Color custom_data_background_color,
                                         std::vector<Custom_Data_IO_Window::message> &message_list,
                                         Graphic_Warehouse& graphic_warehouse):
        buttons_field(std::move(buttons_field_)),

        custom_data_io_window(
                std::make_unique<Custom_Data_IO_Window>(
                        custom_data_view_pos,
                        custom_data_view_size,
                        character_size,
                        custom_data_background_color
                ))
{

    for (auto &message: message_list) {
        custom_data_io_window->add_message(message);
    }

}

Connection_Frontend::Connection_Frontend(std::unique_ptr<Buttons_Field> buttons_field_,
                                         sf::Vector2f camera_view_pos,
                                         sf::Vector2f camera_view_size,
                                         Graphic_Warehouse &graphic_warehouse):
        buttons_field(std::move(buttons_field_)),
        camera_view(std::make_unique<sf::RectangleShape>(camera_view_size)),
        camera_view_target_size(camera_view_size),
        texture(graphic_warehouse.get_texture("Camera_view_deafult"))
{

    camera_view->setPosition(camera_view_pos);
    camera_view->setTexture(&texture);
    camera_view->setSize(static_cast<sf::Vector2f>(texture.getSize()));
    resize_shape(static_cast<sf::Vector2u>(camera_view_target_size),
                 camera_view.get());
}

Connection_Frontend::Connection_Frontend(std::unique_ptr<Buttons_Field> buttons_field_,

                                         sf::Vector2f window_pos,
                                         sf::Vector2f window_size,
                                         sf::Vector2f zero_point_pos,
                                         Axes axis_a,
                                         Axes axis_b,
                                         std::vector<std::tuple<cv::Vec2d, cv::Vec2d, cv::Vec2d>>* received_parameters_,

                                         Graphic_Warehouse &graphic_warehouse):
                                         buttons_field(std::move(buttons_field_))
                                         {
                                             add_projection(window_size, window_pos, zero_point_pos , axis_a, axis_b, received_parameters_);

                                         }


Button::Button_Message Connection_Frontend::update_st(sf::Vector2i mouse_pos_relative_to_window) {
    actual_button_mesage = buttons_field->update_field(mouse_pos_relative_to_window);

    return actual_button_mesage;
}

std::vector<sf::Drawable*>& Connection_Frontend::get_figures_list() {
    static std::vector<sf::Drawable*>  list1;
    list1.clear();

    if(camera_view != nullptr)
        list1.push_back(camera_view.get());
    list1.push_back(buttons_field.get());

    auto list2 = buttons_field->get_figures_list();

    list1.insert(list1.end(), list2.begin(), list2.end());

    if(custom_data_io_window != nullptr) {
        list1.push_back(custom_data_io_window.get());
        auto list3 = custom_data_io_window->get_text_list();
        list1.insert(list1.end(), list3.begin(), list3.end());
    }

    if(custom_data_io_window != nullptr) {
        list1.push_back(custom_data_io_window.get());
        auto list3 = custom_data_io_window->get_text_list();
        list1.insert(list1.end(), list3.begin(), list3.end());
    }

    for(auto& projection: projections_windows_list){
        list1.push_back(&projection);
    }

    return list1;
}

std::vector<std::vector<std::unique_ptr<sf::Drawable>>> *Connection_Frontend::get_additional_graphic_lists() {
    return &additional_graphic_of_small_window;
}


void Connection_Frontend::update_image(const sf::Image &new_image) {
    if(camera_view != nullptr) {
        if (new_image.getSize().x > 0 and new_image.getSize().y > 0) {
            if (!texture.loadFromImage(new_image)) {
                std::cout << "Nie można odczytać" << std::endl;
                return;
            }
        }
        static sf::Vector2u last_vector_size;

        if (last_vector_size != new_image.getSize()) {
            camera_view->setSize(static_cast<sf::Vector2f>(texture.getSize()));
            resize_shape(static_cast<sf::Vector2u>(camera_view_target_size), camera_view.get());
            camera_view->setTexture(&texture, true);
        }

        last_vector_size = new_image.getSize();
    }
}

Button::Button_Message Connection_Frontend::get_and_delate_actual_button_mesage() {
    auto message_copy = actual_button_mesage;
    actual_button_mesage = Button::Button_Message::nothing;
    return message_copy;
}

bool Connection_Frontend::custom_data_update_variable_by_name_int(const std::string &name, int value) {
    if(custom_data_io_window == nullptr)
        return false;

    return custom_data_io_window->update_variable_by_name_int(name, value);
}

bool Connection_Frontend::custom_data_update_variable_by_name_float(const std::string &name, float value) {
    if(custom_data_io_window == nullptr)
        return false;

    return custom_data_io_window->update_variable_by_name_float(name, value);
}

void Connection_Frontend::set_button_mode(Button::Button_Message button_type, bool mode) {
    buttons_field->set_button_mode(button_type, mode);
}

void Connection_Frontend::add_projection(sf::Vector2f size, sf::Vector2f pos, sf::Vector2f zero_point_pos,
                                         Axes axis_a,
                                         Axes axis_b,
                                         std::vector<std::tuple<cv::Vec2d, cv::Vec2d, cv::Vec2d>>* rays_ratio_,
                                         sf::Color background_color,
                                         sf::Color outline_color, float outline_thickness, int update_time) {

    std::vector<std::unique_ptr<sf::Drawable>> additional_graphic_list;

    Small_window projections_window(pos, size, background_color, outline_color, outline_thickness, update_time);

    additional_graphic_of_small_window.push_back(std::move(additional_graphic_list));
    projections_window.set_additional_graphic(&additional_graphic_of_small_window.back());

    Projection_image_calculator new_projection_calculator(
            axis_a,
            axis_b,

            Configs::computers_enum::hp,

            pos,
            size,
            zero_point_pos,
            true,
            rays_ratio_
    );


    new_projection_calculator.set_additional_drawable_ptr(&additional_graphic_of_small_window.back());


    projections_windows_list.push_back(std::move(projections_window));
    projection_calculators.push_back(std::move(new_projection_calculator));
}
