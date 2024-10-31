//
// Created by wpr on 16.08.24.
//
#include <string>
#include <utility>
#include <opencv2/opencv.hpp>
#include <SFML/Window.hpp>
#include "../Time_Object.hpp"

#ifndef MAGISTERKA_IMAGE_SOURCE_HPP
#define MAGISTERKA_IMAGE_SOURCE_HPP


class Image_source: public Time_Object{
public:
    enum class Image_source_typ{
        file,
        camera
    };

    enum class Frame_switching{
        automatic,
        manual
    };

    Image_source(const std::string& file_path_,
                 Frame_switching switching_method_,
                 sf::Keyboard::Key action_key_ = sf::Keyboard::Space);

    explicit Image_source(Frame_switching switching_method_,
                          sf::Keyboard::Key action_key_ = sf::Keyboard::Space);

    void update() override;
    bool need_update() override{return true;};

    void set_image_and_main_time_ptr(std::shared_ptr<std::pair<sf::Time, cv::Mat>> image_and_main_time_);
private:
    Frame_switching switching_method;
    Image_source_typ source_typ;
    cv::VideoCapture cap;
    std::shared_ptr<std::pair<sf::Time, cv::Mat>> last_with_main_time_frame = nullptr;
    sf::Keyboard::Key action_key;
};


#endif //MAGISTERKA_IMAGE_SOURCE_HPP
