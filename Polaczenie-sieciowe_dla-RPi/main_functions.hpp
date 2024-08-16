//
// Created by rodzi on 23.10.2021.
//

#ifndef SR_1_1_MAIN_FUNCTIONS_HPP
#define SR_1_1_MAIN_FUNCTIONS_HPP

#include <opencv2/opencv.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

inline void write_comunicate_sockte_status(sf::Socket::Status status){
    switch (status){
        case sf::Socket::Disconnected:
            std::cout<<"Nie polaczono"<<std::endl;
            break;
        case sf::Socket::Error :
            std::cout<<"Blad"<<std::endl;
            break;
        case sf::Socket::NotReady :
            std::cout<<"Nie gotowy"<<std::endl;
            break;
        case sf::Socket::Partial :
            std::cout<<"Czesciowo polaczony"<<std::endl;
            break;
        case sf::Socket::Done:
            std::cout<<"Polacznie poprawne"<<std::endl;
            break;
    }
}
inline void cv_Mat_2_sf_Image(cv::Mat& cv_img, sf::Image& sf_img){
    //sf::Image sf_img;
//    std::make_unsigned<sf::Image> sf_img;
    cv::Mat cv_img_a;
    cv::cvtColor(cv_img, cv_img_a, cv::COLOR_BGR2RGBA);
    sf_img.create(cv_img_a.cols, cv_img_a.rows, cv_img_a.ptr());
}

inline void resize_sprite(sf::Vector2u target_size, sf::Sprite& sprite) {
    sf::Vector2u orginal_size = sprite.getTexture()->getSize();
    sf::Vector2f convert_vector = sf::Vector2f((float) target_size.x / (float) orginal_size.x,
                                               (float) target_size.y / (float) orginal_size.y);
    sprite.setScale(convert_vector);
}

inline void resize_shape(sf::Vector2u target_size, sf::Shape* shape) {
    sf::Vector2u orginal_size = shape->getTexture()->getSize();
    sf::Vector2f convert_vector = sf::Vector2f((float) target_size.x / (float) orginal_size.x,
                                               (float) target_size.y / (float) orginal_size.y);
    shape->setScale(convert_vector);
}



#endif //SR_1_1_MAIN_FUNCTIONS_HPP
