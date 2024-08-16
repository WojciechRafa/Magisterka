//
// Created by wr on 11/24/22.
//

#include "Window.hpp"

#include <iostream>

Window::Window(sf::Vector2<int> size, const std::string& name):
sf::RenderWindow(sf::VideoMode(size.x, size.y), name)
{

}


