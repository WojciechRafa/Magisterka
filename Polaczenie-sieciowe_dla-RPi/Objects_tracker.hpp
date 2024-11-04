//
// Created by wpr on 25.10.24.
//

#ifndef MAGISTERKA_OBJECTS_TRACKER_HPP
#define MAGISTERKA_OBJECTS_TRACKER_HPP

#include "Time_Object.hpp"
//#include <vector>
#include <map>
#include <opencv2/core/matx.hpp>
#include "GUI/Small_window.hpp"

class Objects_tracker: public Time_Object{
public:
    void add_detecion(sf::Int64 time, std::vector<cv::Vec3d>& pos_vector, std::vector<double>& size_vector);
    std::map<sf::Int64, std::vector<std::pair<cv::Vec3d, double>>>& get_actual_objets_list();

    void set_small_window(Small_window* small_window_ptr_);
    void update() final;

    std::string get_name() override{return "Object Tracker";};
private:
    std::map<sf::Int64, std::vector<std::pair<cv::Vec3d, double>>> detected_objets;
    Small_window* small_window_ptr = nullptr;

    std::vector<std::unique_ptr<sf::Shape>> list_of_graphic;

    sf::Vector2f get_position_of_detected_object_on_main_window(bool& is_in_board, double first_dimension, double second_dimension);
};


#endif //MAGISTERKA_OBJECTS_TRACKER_HPP
