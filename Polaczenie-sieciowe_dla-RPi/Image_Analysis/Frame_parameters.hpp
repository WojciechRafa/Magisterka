//
// Created by wpr on 29.09.24.
//

#ifndef MAGISTERKA_FRAME_PARAMETERS_HPP
#define MAGISTERKA_FRAME_PARAMETERS_HPP

#include <SFML/System/Time.hpp>
#include "../Rays_source.hpp"

struct Object_parameters{
    cv::Vec2d bb_position;
    cv::Vec2d bb_size;
    cv::Vec2d centroid;
};

struct Frame_parameters {
    sf::Int64 time;
    Rays_source* source_ptr;
    std::vector<Object_parameters> objets;
};

#endif //MAGISTERKA_FRAME_PARAMETERS_HPP
