//
// Created by wr on 12/17/22.
//

#ifndef INZYNIERKA_CONNECTION_HPP
#define INZYNIERKA_CONNECTION_HPP

#include "Connection_Frontend.hpp"
#include "Connection_Backend.hpp"

#include "Projection_image_calculator.hpp"
#include "Projection_image_calculator.hpp"

#include <list>

class Connection {
public:
    struct Projection_window_parameters{
        sf::Vector2f position = sf::Vector2f(0, 0);
        sf::Vector2f size = sf::Vector2f(0, 0);
        Projection_image_calculator::axes axis_a = Projection_image_calculator::axes::z;
        Projection_image_calculator::axes axis_b = Projection_image_calculator::axes::x;
        sf::Color background_color = sf::Color::White;
    };

    [[maybe_unused]] Connection(
            std::unique_ptr<Buttons_Field> button_filed_,

            sf::Vector2f custom_data_view_pos,
            sf::Vector2f custom_data_view_size,
            unsigned int character_size,
            sf::Color custom_data_background_color,
            std::vector<Custom_Data_IO_Window::message>& message_list_displayed,
            std::vector<Custom_Data_IO_Window::message>& message_list_sended,

            sf::Vector2f camera_view_pos,
            sf::Vector2f camera_view_size,
            Graphic_Warehouse& graphic_warehouse,
            unsigned short port
            );

    // bez wyświetlania grafik z kamery
    [[maybe_unused]] Connection(
            std::unique_ptr<Buttons_Field> button_filed_,

            sf::Vector2f custom_data_view_pos,
            sf::Vector2f custom_data_view_size,
            unsigned int character_size,
            sf::Color custom_data_background_color,
            std::vector<Custom_Data_IO_Window::message>& message_list_displayed,
            std::vector<Custom_Data_IO_Window::message>& message_list_sended,

            Graphic_Warehouse& graphic_warehouse,
            unsigned short port
    );

    // bez wyświetlania danych liczbowych
    [[maybe_unused]] Connection(
            std::unique_ptr<Buttons_Field> button_filed_,

            std::vector<Custom_Data_IO_Window::message>& message_list_displayed,
            std::vector<Custom_Data_IO_Window::message>& message_list_sended,

            sf::Vector2f camera_view_pos,
            sf::Vector2f camera_view_size,
            Graphic_Warehouse& graphic_warehouse,
            unsigned short port
    );

    // do wyświetlania potencjalnych punktow kamery
    [[maybe_unused]] Connection(
            std::unique_ptr<Buttons_Field> button_filed_,
//            const std::vector<Projection_window_parameters>&,

            sf::Vector2f window_pos,
            sf::Vector2f window_size,
            sf::Vector2f zero_point_pos,

            Projection_image_calculator::axes axis_a,
            Projection_image_calculator::axes axis_b,

            Graphic_Warehouse& graphic_warehouse,
            unsigned short port
    );

    std::vector<sf::Drawable*>&  get_figures_list();
    std::vector<std::vector<std::unique_ptr<sf::Shape>>>* get_additional_graphic_lists();

    //st - short time -  wykorzystywane
    Button::Button_Message update_backend_st();
    Button::Button_Message update_frontend_st(sf::Vector2i mouse_pos_relative_to_window);

    // aktualizowane przy aktualizacji czasowej (dla optymalności)
    void update_image();

    // funkcja wykorzystywana do zarzadzeniem czasem
    std::vector<Time_Object*> get_time_objects();

private:
    Connection_Frontend frontend;
    Connection_Backend backend;

    Button::Button_Message actual_button_mesage = Button::Button_Message::nothing;

    std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>> axes_ratio = {};


};


#endif //INZYNIERKA_CONNECTION_HPP
