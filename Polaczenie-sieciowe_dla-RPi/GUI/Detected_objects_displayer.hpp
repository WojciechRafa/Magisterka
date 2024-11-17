//
// Created by wpr on 15.11.24.
//

#ifndef MAGISTERKA_DETECTED_OBJECTS_DISPLAYER_HPP
#define MAGISTERKA_DETECTED_OBJECTS_DISPLAYER_HPP

#include "../Time_Object.hpp"
#include "../Objects_tracker.hpp"

class Detected_objects_displayer: public Time_Object{
public:
    Detected_objects_displayer( std::pair<Axes, Axes>axes_,
                                cv::Vec2d area_size_,
                                cv::Vec2d zero_point_,
                                Objects_tracker* objects_tracker_ptr_ = nullptr,
                                Small_window* small_window_ptr = nullptr);

    void update() override;
private:
    std::pair<Axes, Axes>axes;
    cv::Vec2d area_size;
    cv::Vec2d zero_point;

    Objects_tracker* objects_tracker_ptr = nullptr;
    Small_window* small_window_ptr = nullptr;

    void update_triangulated_object();
    void update_verified_object();

    void add_detection_graphic(sf::Vector2f pos, float size, sf::Color color);

    std::vector<std::unique_ptr<sf::Shape>> all_graphics = {};
    std::vector<std::unique_ptr<sf::Shape>> detection_graphics = {};
    std::vector<sf::RectangleShape> reference_lines = {};

    void write_grid();
};


#endif //MAGISTERKA_DETECTED_OBJECTS_DISPLAYER_HPP
