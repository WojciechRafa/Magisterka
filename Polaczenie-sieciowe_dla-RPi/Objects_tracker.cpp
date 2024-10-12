//
// Created by wpr on 30.09.24.
//

#include "Objects_tracker.hpp"

void Objects_tracker::set_detecion(cv::Vec3d position, double size, sf::Int64 time) {

}

std::vector<std::tuple<int, cv::Vec3d, double, sf::Int64>> &Objects_tracker::get_actual_objets_list() {
    static std::vector<std::tuple<int, cv::Vec3d, double, sf::Int64>> tempoeraty = {};
    return tempoeraty;
}
