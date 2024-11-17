//
// Created by wr on 12/17/22.
//

#ifndef INZYNIERKA_CONNECTION_HPP
#define INZYNIERKA_CONNECTION_HPP

#include "Connection_Frontend.hpp"
#include "Connection_Backend.hpp"

#include "../Image_Analysis/Projection_image_calculator.hpp"
#include "../Image_Analysis/Projection_image_calculator.hpp"

#include <list>

class Connection {
public:
    struct Projection_window_parameters{
        sf::Vector2f position = sf::Vector2f(0, 0);
        sf::Vector2f size = sf::Vector2f(0, 0);
        Axes axis_a = Axes::z;
        Axes axis_b = Axes::x;
        sf::Color background_color = sf::Color::White;
    };

    // do wyświetlania potencjalnych punktow kamery
    Connection(
            std::unique_ptr<Buttons_Field> button_filed_,

            sf::Vector2f window_pos,
            sf::Vector2f window_size,
            sf::Vector2f zero_point_pos,

            Axes axis_a,
            Axes axis_b,

            Graphic_Warehouse& graphic_warehouse,
            unsigned short port,
            sf::Clock& clock_,
            Rays_intersection_analyzer* rays_intersection_analyzer_ptr_
    );

    std::vector<sf::Drawable*>&  get_figures_list();
    std::vector<std::vector<std::unique_ptr<sf::Shape>>>* get_additional_graphic_lists();

    //st - short time -  wykorzystywane
    Button::Button_Message update_backend_st();
    Button::Button_Message update_frontend_st(sf::Vector2i mouse_pos_relative_to_window);

    // aktualizowane przy aktualizacji czasowej (dla optymalności)
    void update_image();
private:
    Connection_Frontend frontend;
    Connection_Backend backend;

    Button::Button_Message actual_button_mesage = Button::Button_Message::nothing;

    std::vector<std::tuple<cv::Vec2d, cv::Vec2d, cv::Vec2d>> sent_parameters = {};
};


#endif //INZYNIERKA_CONNECTION_HPP
