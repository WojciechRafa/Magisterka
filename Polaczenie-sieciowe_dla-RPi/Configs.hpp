//
// Created by wpr on 27.10.24.
//

#ifndef MAGISTERKA_CONFIGS_HPP
#define MAGISTERKA_CONFIGS_HPP

#include <SFML/Config.hpp>
#include <SFML/Graphics/Color.hpp>
#include <opencv4/opencv2/core.hpp>
#include <map>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

enum class Axes {
    x, y, z
};

static int get_axi_nr(Axes axis) {
    switch (axis) {
        case Axes::x:
            return 0;
        case Axes::y:
            return 1;
        case Axes::z:
            return 2;
        default:
            return -1;
    }
}

namespace Configs{
    static sf::Time default_update_time = sf::milliseconds(50);

    static sf::Time camera_update_period = sf::milliseconds(50);
    static sf::Time max_camera_update_period_error = sf::milliseconds(5);

    static sf::Time main_loop_time = sf::milliseconds(1);


    static bool is_binarization_relative = false;
    static int binarization_threshold = 50;

    namespace GUI_layout{
        static const sf::Vector2f default_small_window_size = sf::Vector2f(360, 230);

        static const sf::Vector2i render_window_size(1200,  800);
        static const std::string render_window_name("Inzynierka");

        static const bool is_projection_calculator_displayed = true;
        static const std::pair<Axes, Axes> projection_calculator_axes(Axes::z, Axes::x);


        static const sf::FloatRect main_button_field(10, 10, 350, 100);
        static const sf::Color main_button_field_color = sf::Color::White;

        static const sf::Vector2f camera_view_window_pos(400, 30);
        static const sf::Vector2f binarization_window_pos(camera_view_window_pos.x + default_small_window_size.x + 10,
                                                          camera_view_window_pos.y);
        static const sf::Vector2f rays_window_pos(camera_view_window_pos.x,
                                                  camera_view_window_pos.y + default_small_window_size.y + 10);

    }
    enum class computers_enum{
        dell,
        hp
    };

    static computers_enum this_computer = computers_enum::hp;

    static std::map<computers_enum, std::string> hw_folder_folders_name{
            {computers_enum::dell, "Dell"},
            {computers_enum::hp, "Hp"},
    };
}

#endif //MAGISTERKA_CONFIGS_HPP
