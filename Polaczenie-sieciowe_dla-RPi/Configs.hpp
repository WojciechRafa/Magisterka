//
// Created by wpr on 27.10.24.
//

#ifndef MAGISTERKA_CONFIGS_HPP
#define MAGISTERKA_CONFIGS_HPP

#include <SFML/Config.hpp>

namespace Configs{
    static sf::Time default_update_time = sf::milliseconds(50);

    static sf::Time camera_update_period = sf::milliseconds(50);
    static sf::Time max_camera_update_period_error = sf::milliseconds(3);

    static sf::Time main_loop_time = sf::milliseconds(1);


    static bool is_binarization_relative = false;
    static int binarization_threshold = 50;
}

#endif //MAGISTERKA_CONFIGS_HPP
