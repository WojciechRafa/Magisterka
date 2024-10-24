//
// Created by wr on 12/17/22.
//

#ifndef INZYNIERKA_CONNECTION_FRONTEND_HPP
#define INZYNIERKA_CONNECTION_FRONTEND_HPP

#include "GUI/Buttons_Field.hpp"
#include "GUI/Graphic_Warehouse.hpp"
#include "GUI/Custom_Data_IO_Window.hpp"
#include "Image_Analysis/Projection_image_calculator.hpp"
#include "GUI/Small_window.hpp"

#include <list>
#include <vector>

class Connection_Frontend {
public:
    // konstruktor pełny
    [[maybe_unused]] Connection_Frontend(
            std::unique_ptr<Buttons_Field> buttons_field_,

            sf::Vector2f custom_data_view_pos,
            sf::Vector2f custom_data_view_size,
            unsigned int character_size,
            sf::Color custom_data_background_color,
            std::vector<Custom_Data_IO_Window::message>& message_list,

            sf::Vector2f camera_view_pos,
            sf::Vector2f camera_view_size,
            Graphic_Warehouse& graphic_warehouse
            );
    // konstruktor bez widoku z kamery
    [[maybe_unused]] Connection_Frontend(
            std::unique_ptr<Buttons_Field> buttons_field_,

            sf::Vector2f custom_data_view_pos,
            sf::Vector2f custom_data_view_size,
            unsigned int character_size,
            sf::Color custom_data_background_color,
            std::vector<Custom_Data_IO_Window::message>& message_list,

            Graphic_Warehouse& graphic_warehouse
    );

    // konstruktor bez okna danych liczbowych
    [[maybe_unused]] Connection_Frontend(
            std::unique_ptr<Buttons_Field> buttons_field_,

            sf::Vector2f camera_view_pos,
            sf::Vector2f camera_view_size,
            Graphic_Warehouse& graphic_warehouse
    );


    [[maybe_unused]] Connection_Frontend(
            std::unique_ptr<Buttons_Field> buttons_field_,

            sf::Vector2f window_pos,
            sf::Vector2f window_size,
            sf::Vector2f zero_point_pos,
            Projection_image_calculator::axes axis_a,
            Projection_image_calculator::axes axis_b,
            std::vector<std::tuple<cv::Vec2d, cv::Vec2d, cv::Vec2d>>* received_parameters_,

            Graphic_Warehouse& graphic_warehouse
    );

    std::vector<sf::Drawable*>& get_figures_list();
    std::vector<std::vector<std::unique_ptr<sf::Shape>>>* get_additional_graphic_lists();

    Button::Button_Message update_st(sf::Vector2i mouse_pos_relative_to_window);// zwraca informację o tym, czy połączenie ma być zakończone
    void update_image(const sf::Image& new_image); // zwraca false przy braku nowego zdjęcia

    // uwaga ! funkcja po wykonaniu zmienia actual button mesage na nothing
    Button::Button_Message get_and_delate_actual_button_mesage();

    bool custom_data_update_variable_by_name_int(const std::string& name, int value);
    bool custom_data_update_variable_by_name_float(const std::string& name, float value);

    void set_button_mode(Button::Button_Message button_type, bool mode);

//    void set_axes_ratio(std::shared_ptr<std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>>> axes_ratio_);

    void add_projection(sf::Vector2f size_,
                        sf::Vector2f pos,
                        sf::Vector2f zero_point_pos,
                        Projection_image_calculator::axes axis_a,
                        Projection_image_calculator::axes axis_b,
                        std::vector<std::tuple<cv::Vec2d, cv::Vec2d, cv::Vec2d>>* detected_object_parameters_,
                        cv::Mat internal_matrix,
                        cv::Mat external_matrix,
                        sf::Color background_color= sf::Color::White,
                        sf::Color outline_color= sf::Color::Black,
                        float outline_thickness = 3,
                        int update_time = 50000);

    std::vector<Time_Object *> get_time_object_list();
private:

    std::unique_ptr<Buttons_Field> buttons_field;
    std::unique_ptr<sf::RectangleShape> camera_view = nullptr;
    std::unique_ptr<Custom_Data_IO_Window> custom_data_io_window = nullptr;

    sf::Texture texture;

    Button::Button_Message actual_button_mesage = Button::Button_Message::nothing;

    sf::Vector2f camera_view_target_size = sf::Vector2f(0, 0);

    std::shared_ptr<std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>>> axes_ratio = nullptr;
    std::vector<Small_window> projections_windows_list;

    std::vector<Projection_image_calculator> projection_calculators;
    std::vector<std::vector<std::unique_ptr<sf::Shape>>> additional_graphic_of_small_window = {};

};


#endif //INZYNIERKA_CONNECTION_FRONTEND_HPP
