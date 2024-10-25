//
// Created by wpr on 11.12.22.
//

#include "Button.hpp"

Button::Button(sf::Vector2f position_,
               sf::Vector2f button_field_position_,
               sf::Vector2f size_,
               Button_Message button_messages_,
               sf::Texture& texture_,
               float outline_thickness_,
               sf::Color outline_color_makred_,
               sf::Color outline_color_active_,
               sf::Color outline_color_makred_and_active_):

               sf::RectangleShape(size_),

               reaction_message(button_messages_),
               texture(texture_),

               outline_color_makred(outline_color_makred_),
               outline_color_active(outline_color_active_),
               outline_color_makred_and_active(outline_color_makred_and_active_)
               {
    setPosition(position_ + button_field_position_);
    setOutlineThickness(outline_thickness_);
    setOutlineColor(sf::Color::Transparent);

    setTexture(&texture);
}

bool Button::is_makred() const {
    return is_marked;
}

bool Button::is_activeted() const {
    return is_active;
}

Button::Button_Message Button::is_active_get_message() const {
    if(is_active)
        return reaction_message;
    else
        return Button_Message::nothing;
}

Button::Button_Message Button::get_message() const {
    return reaction_message;
}

Button::Button_Message Button::check_mouse_in_and_update_colors(sf::Vector2f mouse_pos, bool is_pressed) {


    if(mouse_pos.x > getPosition().x and mouse_pos.y > getPosition().y){
        if(mouse_pos.x < getPosition().x + getSize().x * getScale().x and mouse_pos.y < getPosition().y + getSize().y * getScale().y) {
            is_marked = true;

            if(is_pressed) {
                is_active = true;
                update_outline();
                return reaction_message;
            }
            update_outline();
            return Button_Message::nothing;
        }
    }
    is_marked = false;
    update_outline();
    return Button_Message::nothing;
}

void Button::update_outline() {
    if(not is_marked and not is_active)
        setOutlineColor(sf::Color::Transparent);
    else if(is_marked and not is_active)
        setOutlineColor(outline_color_makred);
    else if(not is_marked and is_active)
        setOutlineColor(outline_color_active);
    else if(is_marked and is_active)
        setOutlineColor(outline_color_makred_and_active);
}

void Button::decactivate() {
    is_active = false;
}

void Button::activate() {
    is_active = true;
}




