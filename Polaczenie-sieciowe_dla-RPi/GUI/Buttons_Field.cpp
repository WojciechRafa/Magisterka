//
// Created by wpr on 11.12.22.
//

#include "Buttons_Field.hpp"

Buttons_Field::Buttons_Field(
                             sf::Vector2f position_,
                             sf::Vector2f size_,
                             sf::Color background_color_,
                             float outline_thickness_,
                             sf::Color outline_color_
                             ):
                             sf::RectangleShape(size_) {
    setPosition(position_);
    setFillColor(background_color_);
    setOutlineThickness(outline_thickness_);
    setOutlineColor(outline_color_);
}



Button::Button_Message Buttons_Field::update_field(sf::Vector2i mouse_pos_relative_to_window) {
    Button::Button_Message message = Button::Button_Message::nothing;
    for(auto& button: buttons_list){
        auto result = button.check_mouse_in_and_update_colors(static_cast<sf::Vector2f>(mouse_pos_relative_to_window), sf::Mouse::isButtonPressed(sf::Mouse::Button::Left));
        if (result != Button::Button_Message::nothing)
            message = result;
    }
    return message;
}



void Buttons_Field::add_button(Button &&button) {
    buttons_list.push_back(button);
}

std::vector<Button*> Buttons_Field::get_figures_list() {
    std::vector<Button*> list;
    for(auto& button: buttons_list){
        list.push_back(&button);
    }
    return list;
}

void Buttons_Field::set_button_mode(Button::Button_Message button_type, bool mode) {
    for(auto& button : buttons_list){
        if(button.get_message() == button_type){
            if(mode){
                button.activate();
            }else{
                button.decactivate();
            }
        }
    }
}

