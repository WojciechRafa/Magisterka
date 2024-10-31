//
// Created by wpr on 16.08.24.
//

#include "Image_source_typ.hpp"

#include <utility>

Image_source::Image_source(const std::string& file_path_,
                           Frame_switching switching_method_,
                           int change_time_,
                           sf::Keyboard::Key action_key_) :
        switching_method(switching_method_),
        source_typ(Image_source_typ::file),
        cap(file_path_),
        Time_Object(change_time_),
        action_key(action_key_)
{}

Image_source::Image_source(Frame_switching switching_method_,
                           int change_time_,
                           sf::Keyboard::Key action_key_):
        switching_method(switching_method_),
        source_typ(Image_source_typ::camera),
        cap(0),
        Time_Object(change_time_),
        action_key(action_key_)
{}

void Image_source::update()
{
    if(switching_method == Frame_switching::automatic){
        cap >> last_with_main_time_frame->second;  // if move is ended, last_with_main_time_frame.empty() is true

    }else{
        static bool was_action_button_released = true;

        if(was_action_button_released and sf::Keyboard::isKeyPressed(action_key)){
            was_action_button_released = false;
            cap >> last_with_main_time_frame->second;
        }

    }

    last_update_time = clock.getElapsedTime().asMicroseconds();
}

void Image_source::set_image_and_main_time_ptr(std::shared_ptr<std::pair<sf::Time, cv::Mat>> image_and_main_time_) {
    last_with_main_time_frame = std::move(image_and_main_time_);
}
