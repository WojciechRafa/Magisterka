//
// Created by wpr on 27.10.24.
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
    static sf::Time max_camera_update_period_error = sf::milliseconds(3);

    static sf::Time main_loop_time = sf::milliseconds(1);


    static bool is_binarization_relative = false;
    static int binarization_threshold = 50;

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
