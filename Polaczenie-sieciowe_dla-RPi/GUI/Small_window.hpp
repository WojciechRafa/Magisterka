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
    explicit Small_window(sf::Vector2f pos_,
                 sf::Vector2f size_ = Configs::GUI_layout::default_small_window_size,
                 sf::Color background_color= sf::Color::White,
                 sf::Color outline_color= sf::Color::Black,
                 float outline_thickness = 3,
                 int update_time = 50000);

    ~Small_window() override = default;

    Small_window(Small_window&&) noexcept = default;
    Small_window& operator=(Small_window&&) noexcept = default;

    void update();
    void set_image_ptr(cv::Mat* image_);

    void set_additional_graphic(std::vector<std::unique_ptr<sf::Shape>>* additional_graphic_);
    std::vector<std::unique_ptr<sf::Shape>>* get_additional_graphic();
//    std::vector<sf::Drawable*>& get_additional_graphic();
    std::string get_name() override{return "Small Window";};
private:
    cv::Mat* image_cv_ptr = nullptr;
    cv::Mat image_copy;
    sf::Vector2f size;

    sf::Texture texture;

    std::vector<std::unique_ptr<sf::Shape>>* additional_graphic = nullptr;

};


#endif //MAGISTERKA_SMALL_WINDOW_HPP
