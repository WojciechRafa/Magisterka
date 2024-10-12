//
// Created by wpr on 30.09.24.
//

#ifndef MAGISTERKA_OBJECTS_TRACKER_HPP
#define MAGISTERKA_OBJECTS_TRACKER_HPP

#include <vector>
#include <opencv2/core/matx.hpp>
#include <SFML/System/Time.hpp>

class Objects_tracker {
public:
    void set_detecion(cv::Vec3d position, double size, sf::Int64 time);
    std::vector<std::tuple<int, cv::Vec3d, double, sf::Int64>>& get_actual_objets_list();

private:
    std::vector<std::tuple<int, bool, std::vector<cv::Vec3d>, double, sf::Int64>> detected_objets;
};


#endif //MAGISTERKA_OBJECTS_TRACKER_HPP
