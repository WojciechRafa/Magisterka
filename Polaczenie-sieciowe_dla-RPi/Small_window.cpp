//
// Created by wpr on 16.08.24.
//

#include "Small_window.hpp"

#include <utility>

Small_window::Small_window(sf::Vector2f size_,
                           sf::Vector2f pos,
                           sf::Color background_color,
                           int update_time) :
    Time_Object(update_time),
    size(size_)
{
    setPosition(pos);
    setSize(size);
//    setColor(background_color);
    setFillColor(background_color);
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

            if (image_copy.type() != CV_8UC3) {
                std::cerr << "Error: Image is not of type CV_8UC3." << std::endl;
            }

            std::cout << "Rows: " << image_cv_ptr->rows << std::endl;
            std::cout << "Cols: " << image_cv_ptr->cols << std::endl;
            std::cout << "Size: " << image_cv_ptr->size() << std::endl;
            std::cout << "Channels: " << image_cv_ptr->channels() << std::endl;
            std::cout << "Type: " << getMatType(*image_cv_ptr) << std::endl;
            std::cout << "Step: " << image_cv_ptr->step << " bytes" << std::endl;
            std::cout << "Element Size: " << image_cv_ptr->elemSize() << " bytes" << std::endl;
            std::cout << "Is empty: " << std::boolalpha << image_cv_ptr->empty() << std::endl;

            last_update_time = clock.getElapsedTime().asMicroseconds();
            return;
        }
    }
    last_update_time = clock.getElapsedTime().asMicroseconds();
}

void Small_window::set_image_ptr(std::shared_ptr<cv::Mat> image_ptr_) {
    image_cv_ptr = image_ptr_;
}
