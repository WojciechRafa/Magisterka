//
// Created by wpr on 16.08.24.
//
#include <SFML/Graphics.hpp>
#include <opencv2/opencv.hpp>
#include "Time_Object.hpp"
#include "vector"

#ifndef MAGISTERKA_SMALL_WINDOW_HPP
#define MAGISTERKA_SMALL_WINDOW_HPP


class Small_window: public sf::RectangleShape, public Time_Object{
public:
    Small_window(sf::Vector2f size_,
                 sf::Vector2f pos,
                 sf::Color background_color= sf::Color::Black,
                 int update_time = 30000);

    void update() override;
    void set_image_ptr(std::shared_ptr<cv::Mat> image_);
private:
    std::shared_ptr<cv::Mat> image_cv_ptr = nullptr;
    cv::Mat image_copy;
    sf::Vector2f size;

    sf::Texture texture;
};


#endif //MAGISTERKA_SMALL_WINDOW_HPP
