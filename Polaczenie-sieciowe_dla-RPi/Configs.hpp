//
// Created by wpr on 24.10.24.
//

#ifndef MAGISTERKA_CONFIGS_HPP
#define MAGISTERKA_CONFIGS_HPP

#include <SFML/Config.hpp>
#include <SFML/Graphics/Color.hpp>
#include "Image_Analysis/Projection_image_calculator.hpp"

namespace Configs{
    static sf::Int64 default_update_time = 50000;

    static sf::Int64 camera_update_period_microseconds = 50000;
    static double max_intersection_size_comparison = 2;

    static bool is_binarization_relative = false;
    static int binarization_threshold = 50;

    namespace Big_window_parameters{
        static sf::Int64 displayed_time_microseconds = 500000;
        static cv::Vec2d size_meters = cv::Vec2d(20, 10);
        static cv::Vec2d zero_point_meters = cv::Vec2d(10, 5);

        static float circle_radius_relation = 3;
        static sf::Color circle_color = sf::Color::Magenta;

        static Projection_image_calculator::axes first_axle = Projection_image_calculator::axes::x;
        static Projection_image_calculator::axes second_axle = Projection_image_calculator::axes::y;
    }

}

#endif //MAGISTERKA_CONFIGS_HPP
