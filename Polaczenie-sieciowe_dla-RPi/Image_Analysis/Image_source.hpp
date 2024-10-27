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
                 int change_time_ = 50000,
                 sf::Keyboard::Key action_key_ = sf::Keyboard::Space);

    explicit Image_source(Frame_switching switching_method_,
                          int change_time_ = 50000,
                          sf::Keyboard::Key action_key_ = sf::Keyboard::Space);

    void update() override;

    void set_image_ptr(std::shared_ptr<cv::Mat> image_);
private:
    Frame_switching switching_method;
    Image_source_typ source_typ;
    cv::VideoCapture cap;
    std::shared_ptr<cv::Mat> last_frame = nullptr;
    sf::Keyboard::Key action_key;
};


#endif //MAGISTERKA_IMAGE_SOURCE_HPP
