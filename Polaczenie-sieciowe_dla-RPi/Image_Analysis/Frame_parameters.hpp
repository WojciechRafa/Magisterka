//
// Created by wpr on 29.09.24.
//

#ifndef MAGISTERKA_FRAME_PARAMETERS_HPP
#define MAGISTERKA_FRAME_PARAMETERS_HPP

#include <SFML/System/Time.hpp>
class Rays_source;

struct Object_parameters{
public:
    cv::Vec2d bb_position;
    cv::Vec2d bb_size;
    cv::Vec2d centroid;

    Object_parameters(const cv::Vec2d& bb_position_, const cv::Vec2d& bb_size_, const cv::Vec2d& centroid_):
        bb_position(bb_position_),
        bb_size(bb_size_),
        centroid(centroid_)
    {}

    explicit Object_parameters(std::tuple<cv::Vec2d, cv::Vec2d, cv::Vec2d> data_tuple):
            Object_parameters(
                    std::get<0>(data_tuple),
                    std::get<1>(data_tuple),
                    std::get<2>(data_tuple)
                    )
    {}
};

class Frame_parameters {
public:
    sf::Time time = sf::seconds(0);
    Rays_source* source_ptr = nullptr;
    std::vector<Object_parameters> objets = {};

    Frame_parameters(sf::Time time_, Rays_source* source_ptr_, std::vector<std::tuple<cv::Vec2d, cv::Vec2d, cv::Vec2d>>& data):
    time(time_), source_ptr(source_ptr_){
        for(auto& object_tuple: data){
            objets.emplace_back(object_tuple);
        }
    };
};
#endif //MAGISTERKA_FRAME_PARAMETERS_HPP
