//
// Created by wpr on 29.12.22.
//

#include "Custom_Data_IO_Window.hpp"

Custom_Data_IO_Window::Custom_Data_IO_Window(sf::Vector2f position,
                                             sf::Vector2f size,
                                             unsigned int character_size_,
                                             sf::Color background_color,
                                             float outline_thickness,
                                             sf::Color outline_color,
                                             sf::Color letters_color) {
        setSize(size);
        setPosition(position);
        setFillColor(background_color);
        setOutlineThickness(outline_thickness);
        setOutlineColor(outline_color);

        if(font == nullptr){
            font = std::make_shared<sf::Font>();
            if(not font->loadFromFile("../Czcionka_v1.ttf")){
                throw std::exception();
            }
        }
        character_size = character_size_;
        characters_color = letters_color;
}

void Custom_Data_IO_Window::add_message(Custom_Data_IO_Window::message message) {
    auto distance_between_letters = static_cast<float> (character_size / 4.0);

    std::string displayed_string = get_string(message);
    sf::Text new_text;

    new_text.setFont(*font);
    new_text.setCharacterSize(character_size);
    new_text.setFillColor(characters_color);
    new_text.setString(displayed_string);

    if(message_list.empty()) {
        new_text.setPosition(getPosition() + sf::Vector2f(distance_between_letters, distance_between_letters));
    }else{
        auto last_position = texture_list.back().getPosition();
        new_text.setPosition(last_position + sf::Vector2f(0, distance_between_letters + static_cast<float>(character_size)));
    }
    texture_list.push_back(new_text);
    message_list.push_back(message);
}

std::vector<sf::Text *> Custom_Data_IO_Window::get_text_list() {
    std::vector<sf::Text*> list;
    for(auto& texture: texture_list){
        list.push_back(&texture);
    }
    return list;
}

std::shared_ptr<sf::Font> Custom_Data_IO_Window::font = nullptr;

bool Custom_Data_IO_Window::update_variable_by_name_int(const std::string& name, int value) {

    for(int i=0; i < message_list.size(); i++){
        if(message_list[i].is_int and message_list[i].name == name){
            message_list[i].data_int = value;
            texture_list[i].setString(get_string(message_list[i]));
            return true;
        }
    }

    return false;
}

bool Custom_Data_IO_Window::update_variable_by_name_float(const std::string& name, float value) {
    for(int i=0; i < message_list.size(); i++){
        if(not message_list[i].is_int and message_list[i].name == name){
            message_list[i].data_float = value;
            texture_list[i].setString(get_string(message_list[i]));
            return true;
        }
    }

    return false;
}

std::string Custom_Data_IO_Window::get_string(const Custom_Data_IO_Window::message& message) {
    std::string number_string;
    if(message.is_int){
        number_string = std::to_string(message.data_int);
    }else{
        number_string = std::to_string(message.data_float);
    }
    return message.name + " " + number_string;

}

