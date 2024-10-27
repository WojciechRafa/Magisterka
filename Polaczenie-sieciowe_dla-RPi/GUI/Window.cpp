//
// Created by wpr on 16.08.24.
//

#include "Window.hpp"

#include <iostream>

Window::Window(sf::Vector2<int> size, const std::string& name):
        sf::RenderWindow(sf::VideoMode(size.x, size.y), name)
{

}
