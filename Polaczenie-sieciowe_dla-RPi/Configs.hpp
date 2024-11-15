//
// Created by wpr on 24.10.24.
//

#ifndef MAGISTERKA_CONFIGS_HPP
#define MAGISTERKA_CONFIGS_HPP

#include <SFML/Config.hpp>
#include <SFML/Graphics/Color.hpp>
#include <opencv4/opencv2/core.hpp>
#include <map>

enum class Axes {
    x, y, z
};

namespace Configs{
    static sf::Time default_update_time = sf::milliseconds(50);

    static sf::Time camera_update_period = sf::milliseconds(50);
    static sf::Time max_camera_update_period_error = sf::milliseconds(30);

    static sf::Time main_loop_time = sf::milliseconds(1);
    static double max_intersection_size_comparison = 10;

    static bool is_binarization_relative = false;
    static int binarization_threshold = 25;

    namespace Big_window_parameters{
        static sf::Time displayed_time = sf::milliseconds(2000);
        static cv::Vec2d size_meters = cv::Vec2d(20, 10);
        static cv::Vec2d zero_point_meters = cv::Vec2d(10, 5);

        static float circle_radius_ratio = 0.000001;
        static sf::Color circle_color = sf::Color::Magenta;

        static float position_ratio = 8;

        static Axes first_axle = Axes::x;
        static Axes second_axle = Axes::y;

        bool is_const_size = true;
        float default_object_size = 10;
    }

    enum class computers_enum{
        dell,
        hp
    };

    static computers_enum local_computer = computers_enum::dell;
    static computers_enum remote_computer = computers_enum::hp;

    static std::map<computers_enum, std::string> hw_folder_folders_name{
            {computers_enum::dell, "Dell"},
            {computers_enum::hp, "Hp"},
    };

    namespace Object_tracker{
        double max_size_proportion = 2;
        double max_distance = 10;
        int min_detect_number = 3;
        sf::Time remove_time = sf::seconds(1);
    }
}

#endif //MAGISTERKA_CONFIGS_HPP
