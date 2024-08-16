//
// Created by wr on 11/24/22.
//

#ifndef INZYNIERKA_WINDOW_HPP
#define INZYNIERKA_WINDOW_HPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>

#include "Graphic_Warehouse.hpp"

// orginalnie miał on dzidziczyć po time object, ale zrezygnowano z tego
class Window: public sf::RenderWindow{
public:
   explicit Window(sf::Vector2<int> size = sf::Vector2<int>(1200,  800),
                    const std::string& name = "Inzynierka");

private:
};


#endif //INZYNIERKA_WINDOW_HPP
