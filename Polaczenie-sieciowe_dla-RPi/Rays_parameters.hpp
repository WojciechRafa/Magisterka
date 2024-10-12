//
// Created by wpr on 29.09.24.
//

#ifndef MAGISTERKA_RAYS_PARAMETERS_HPP
#define MAGISTERKA_RAYS_PARAMETERS_HPP

#include <SFML/System/Time.hpp>
#include "Rays_source.hpp"

struct Rays_parameters {
    sf::Int64 time;
    Rays_source* source_ptr;
    std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>> rays;
};

#endif //MAGISTERKA_RAYS_PARAMETERS_HPP
