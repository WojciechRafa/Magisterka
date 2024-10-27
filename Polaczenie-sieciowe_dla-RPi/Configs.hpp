//
// Created by wpr on 27.10.24.
//

#ifndef MAGISTERKA_CONFIGS_HPP
#define MAGISTERKA_CONFIGS_HPP

#include <SFML/Config.hpp>

namespace Configs{
    static sf::Int64 default_update_time = 50000;

    static sf::Int64 camera_update_period_microseconds = 50000;

    static bool is_binarization_relative = false;
    static int binarization_threshold = 50;
}

#endif //MAGISTERKA_CONFIGS_HPP
