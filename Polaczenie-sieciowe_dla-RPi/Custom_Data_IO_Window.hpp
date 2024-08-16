//
// Created by wpr on 29.12.22.
//

#ifndef INZYNIERKA_CUSTOM_DATA_IO_WINDOW_HPP
#define INZYNIERKA_CUSTOM_DATA_IO_WINDOW_HPP

#include <SFML/Graphics.hpp>
#include <list>
#include <memory>

class Custom_Data_IO_Window: public sf::RectangleShape
        {
public:
    struct message{
        sf::Int32 id;
        std::string name = "Nie nadano nazwy";
        bool is_int; // jeżeli false to float
//        bool is_read_from_remote_device = true;
        sf::Int32 data_int = 0;
        float data_float = 0.0;
    };


    Custom_Data_IO_Window(
                        sf::Vector2f position,
                        sf::Vector2f size,
                        unsigned int character_size_,
                        sf::Color background_color = sf::Color::White,
                        float outline_thickness =2.0,
                        sf::Color outline_color = sf::Color::Black,
                        sf::Color letters_color = sf::Color::Black
                        );

    void add_message(message message);
    std::vector<sf::Text*> get_text_list();

    bool update_variable_by_name_int(const std::string& name, int value);
    bool update_variable_by_name_float(const std::string& name, float value);

    private:
        static std::string get_string(const message& message);

        std::vector<message> message_list;
        std::vector<sf::Text> texture_list;
        unsigned int character_size;
        sf::Color characters_color;

        static std::shared_ptr<sf::Font> font;
};





#endif //INZYNIERKA_CUSTOM_DATA_IO_WINDOW_HPP
