//
// Created by wpr on 16.08.24.
//

#include "Small_window.hpp"

#include <utility>

Small_window::Small_window(sf::Vector2f pos_,
                           sf::Vector2f size_,
                           sf::Color background_color,
                           sf::Color outline_color,
                           float outline_thickness,
                           int update_time) :
    size(size_)
{
    setPosition(pos_);
    setSize(size);
//    setColor(background_color);
    setFillColor(background_color);
    setOutlineColor(outline_color);
    setOutlineThickness(outline_thickness);
}

std::string getMatType(const cv::Mat& mat) {
    int type = mat.type();
    std::string r;

    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (type >> CV_CN_SHIFT);

    switch (depth) {
        case CV_8U:  r = "8U"; break;
        case CV_8S:  r = "8S"; break;
        case CV_16U: r = "16U"; break;
        case CV_16S: r = "16S"; break;
        case CV_32S: r = "32S"; break;
        case CV_32F: r = "32F"; break;
        case CV_64F: r = "64F"; break;
        default:     r = "User"; break;
    }

    r += "C";
    r += (chans + '0');

    return r;
}

void Small_window::update() {
    if(image_cv_ptr != nullptr){
        if(not image_cv_ptr->empty()) {
            image_copy = image_cv_ptr->clone();

            std::vector<uchar> buffer;
            cv::imencode(".png", image_copy, buffer);

            if (!texture.loadFromMemory(buffer.data(), buffer.size())) {
                std::cerr << "Error: Could not load texture from memory" << std::endl;
            }

            setTexture(&texture);

            last_update_time = clock.getElapsedTime();
            return;
        }
    }
    last_update_time = clock.getElapsedTime();
}

void Small_window::set_image_ptr(cv::Mat* image_ptr_) {
    image_cv_ptr = image_ptr_;
}

void Small_window::set_additional_graphic(std::vector<std::unique_ptr<sf::Drawable>>* additional_graphic_) {
    additional_graphic = additional_graphic_;
}

std::vector<std::unique_ptr<sf::Drawable>>* Small_window::get_additional_graphic() {
    return additional_graphic;
}


