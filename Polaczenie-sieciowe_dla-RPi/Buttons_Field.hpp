//
// Created by wpr on 11.12.22.
//

#ifndef INZYNIERKA_BUTTONS_FIELD_HPP
#define INZYNIERKA_BUTTONS_FIELD_HPP

#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include <list>

class Buttons_Field: public sf::RectangleShape {
public:
    Buttons_Field(
                  sf::Vector2f position_,
                  sf::Vector2f size_,
                  sf::Color background_color_,
                  float outline_thickness_ = 1,
                  sf::Color outline_color_ = sf::Color::Black
                  );

    Button::Button_Message update_field(sf::Vector2i mouse_pos_relative_to_window);
    std::vector<Button*> get_figures_list();

    void add_button(Button && button);

    void set_button_mode(Button::Button_Message button_type, bool mode); // tworząc funkcje założono, że jest maksymalnie jeden przycisk danego typu, funkcja zmienia jedynie wizualny aspekt widoczności
private:
    std::vector<Button> buttons_list;
};


#endif //INZYNIERKA_BUTTONS_FIELD_HPP
