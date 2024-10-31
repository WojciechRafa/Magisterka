//
// Created by wpr on 24.10.24.
//

#ifndef MAGISTERKA_CONFIGS_HPP
#define MAGISTERKA_CONFIGS_HPP

#include <SFML/Config.hpp>
#include <SFML/Graphics/Color.hpp>
#include <opencv4/opencv2/core.hpp>

enum class Axes {
    x, y, z
};

namespace Configs{
    static sf::Time default_update_time = sf::milliseconds(50);

    static sf::Time camera_update_period = sf::milliseconds(50);
    static sf::Time max_camera_update_period_error = sf::milliseconds(3);

    static sf::Time main_loop_time = sf::milliseconds(1);
    static double max_intersection_size_comparison = 2;

    static bool is_binarization_relative = false;
    static int binarization_threshold = 50;

    namespace Big_window_parameters{
        static sf::Time displayed_time = sf::milliseconds(50);
        static cv::Vec2d size_meters = cv::Vec2d(20, 10);
        static cv::Vec2d zero_point_meters = cv::Vec2d(10, 5);

        static float circle_radius_relation = 3;
        static sf::Color circle_color = sf::Color::Magenta;

        static Axes first_axle = Axes::x;
        static Axes second_axle = Axes::y;
    }

}

#endif //MAGISTERKA_CONFIGS_HPP
