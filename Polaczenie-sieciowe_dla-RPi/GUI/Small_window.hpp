//
// Created by wpr on 16.08.24.
//
#include <SFML/Graphics.hpp>
#include <opencv2/opencv.hpp>
#include "../Time_Object.hpp"
#include "vector"

#ifndef MAGISTERKA_SMALL_WINDOW_HPP
#define MAGISTERKA_SMALL_WINDOW_HPP


class Small_window: public sf::RectangleShape, public Time_Object{
public:
    Small_window(sf::Vector2f size_,
                 sf::Vector2f pos,
                 sf::Color background_color= sf::Color::White,
                 sf::Color outline_color= sf::Color::Black,
                 float outline_thickness = 3,
                 int update_time = 50000);

    ~Small_window() override = default;

    void update();
    void set_image_ptr(cv::Mat* image_);

    void set_additional_graphic(std::vector<std::unique_ptr<sf::Shape>>* additional_graphic_);
    std::vector<std::unique_ptr<sf::Shape>>* get_additional_graphic();
//    std::vector<sf::Drawable*>& get_additional_graphic();

private:
    cv::Mat* image_cv_ptr = nullptr;
    cv::Mat image_copy;
    sf::Vector2f size;

    sf::Texture texture;

    std::vector<std::unique_ptr<sf::Shape>>* additional_graphic = nullptr;
//    std::vector<std::unique_ptr<sf::Shape>> additional_graphic_local;
//    std::vector<sf::Drawable> additional_graphic_local;
};


#endif //MAGISTERKA_SMALL_WINDOW_HPP
