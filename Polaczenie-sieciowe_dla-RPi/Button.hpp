//
// Created by wpr on 11.12.22.
//

#ifndef INZYNIERKA_BUTTON_HPP
#define INZYNIERKA_BUTTON_HPP

#include <SFML/Graphics.hpp>

class Button: public sf::RectangleShape {
public:
    enum class Button_Message {
        nothing,
        create_new_screen,
        turn_off_program,
        turn_off_connection,
        broadcast_ip_process,
        conection_establish_camera_view,
        conection_establish_custom_data,
        set_camera_mode_const_20_fps,
        set_camera_mode_x1,
        set_camera_mode_x0_5
    };

    Button(sf::Vector2f position_,
           sf::Vector2f button_field_position_,
           sf::Vector2f size_,

           Button_Message button_messages_,
           sf::Texture& texture_,

           float outline_thickness_ = 5,
           sf::Color outline_color_makred_ = sf::Color::Green,
           sf::Color outline_color_active_ = sf::Color::Red,
           sf::Color outline_color_makred_and_active_ = sf::Color::Black
           );

    Button_Message check_mouse_in_and_update_colors(sf::Vector2f mouse_pos, bool is_pressed); // pozycja myszy podawan jest względem Buttons_Field

    bool is_makred() const;
    bool is_activeted() const;
    Button_Message is_active_get_message() const; // zamiast true i false zwraca "message" lub nothing
    Button_Message get_message() const;
    void decactivate();
    void activate();
private:
    bool is_marked = false;
    bool is_active = false;

    sf::Color outline_color_makred;
    sf::Color outline_color_active;
    sf::Color outline_color_makred_and_active;

    Button_Message reaction_message;
    sf::Texture& texture;



    void update_outline();
};


#endif //INZYNIERKA_BUTTON_HPP
