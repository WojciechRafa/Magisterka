//
// Created by wr on 12/17/22.
//

#ifndef INZYNIERKA_CONNECTION_FRONTEND_HPP
#define INZYNIERKA_CONNECTION_FRONTEND_HPP

#include "Buttons_Field.hpp"
#include "Graphic_Warehouse.hpp"
#include "Custom_Data_IO_Window.hpp"

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

    std::vector<sf::Drawable*> get_figures_list();

    Button::Button_Message update_st(sf::Vector2i mouse_pos_relative_to_window);// zwraca informację o tym, czy połączenie ma być zakończone
    void update_image(const sf::Image& new_image); // zwraca false przy braku nowego zdjęcia

    // uwaga ! funkcja po wykonaniu zmienia actual button mesage na nothing
    Button::Button_Message get_and_delate_actual_button_mesage();

    bool custom_data_update_variable_by_name_int(const std::string& name, int value);
    bool custom_data_update_variable_by_name_float(const std::string& name, float value);

    void set_button_mode(Button::Button_Message button_type, bool mode);
private:

    std::unique_ptr<Buttons_Field> buttons_field;
    std::unique_ptr<sf::RectangleShape> camera_view = nullptr;
    std::unique_ptr<Custom_Data_IO_Window> custom_data_io_window = nullptr;

    sf::Texture texture;

    Button::Button_Message actual_button_mesage = Button::Button_Message::nothing;

    sf::Vector2f camera_view_target_size = sf::Vector2f(0, 0);
};


#endif //INZYNIERKA_CONNECTION_FRONTEND_HPP
