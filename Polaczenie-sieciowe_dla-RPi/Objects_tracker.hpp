//
// Created by wpr on 25.10.24.
//

#ifndef MAGISTERKA_OBJECTS_TRACKER_HPP
#define MAGISTERKA_OBJECTS_TRACKER_HPP

#include "Time_Object.hpp"
#include <vector>
#include <map>
#include <opencv2/core/matx.hpp>
#include "GUI/Small_window.hpp"



class Objects_tracker: public Time_Object{
public:
    struct Verified_object{
        std::vector<cv::Vec3d> position_list;
        double size;
        sf::Color color;
        sf::Time last_update;
        bool is_displayed = false;
    };

    void add_detection(sf::Time time, std::vector<cv::Vec3d>& pos_vector, std::vector<double>& size_vector);
    std::map<sf::Time, std::vector<std::pair<cv::Vec3d, double>>>& get_all_triangulated_object();
    std::vector<Verified_object>& get_all_verified_object();

    void update() final;


private:
    std::map<sf::Time, std::vector<std::pair<cv::Vec3d, double>>> triangulated_objets;

    std::vector<Verified_object> verified_object_list;

    Verified_object* get_closed_object(const cv::Vec3d& position, double size);

    static sf::Color get_random_color();
};


#endif //MAGISTERKA_OBJECTS_TRACKER_HPP
