//
// Created by wpr on 15.11.24.
//

#ifndef MAGISTERKA_DETECTED_OBJECTS_DISPLAY_HPP
#define MAGISTERKA_DETECTED_OBJECTS_DISPLAY_HPP

#include "../Time_Object.hpp"
#include "../Objects_tracker.hpp"

class Detected_objects_display: public Time_Object{
public:
//    Detected_objects_display( std::pair<Axes, Axes>axes_,
//                                cv::Vec2d area_size_,
//                                cv::Vec2d zero_point_,
//                                Objects_tracker* objects_tracker_ptr_ = nullptr,
//                                Small_window* small_window_ptr = nullptr);

    Detected_objects_display(Configs::Big_windows_parameters::Displayed_window_configs& config_ ,
                             Objects_tracker* objects_tracker_ptr_ = nullptr,
                             Small_window* small_window_ptr_ = nullptr);

    void update() override;
private:
//    std::pair<Axes, Axes>axes;
//    cv::Vec2d area_size;
//    cv::Vec2d zero_point;
    Configs::Big_windows_parameters::Displayed_window_configs& config;

    Objects_tracker* objects_tracker_ptr = nullptr;
    Small_window* small_window_ptr = nullptr;

    void update_triangulated_object();
    void update_verified_object();

    void add_detection_graphic(sf::Vector2f pos, float size, sf::Color color);

    std::vector<std::unique_ptr<sf::Drawable>> all_graphics = {};
    std::vector<std::unique_ptr<sf::Shape>> detection_graphics = {};
//    std::vector<sf::Drawable> reference_lines = {};
    sf::VertexArray reference_lines;

    void create_grid();

    sf::Vector2f get_position_of_detected_object_on_window(bool& is_in_board,
                                                           double first_dim_3d,
                                                           double second_dim_3d);
};


#endif //MAGISTERKA_DETECTED_OBJECTS_DISPLAY_HPP
