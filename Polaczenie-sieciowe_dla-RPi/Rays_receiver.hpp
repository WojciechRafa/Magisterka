//
// Created by wpr on 24.08.24.
//

#ifndef MAGISTERKA_RAYS_RECEIVER_HPP
#define MAGISTERKA_RAYS_RECEIVER_HPP

#include <opencv2/opencv.hpp>

#include "Permanent_Connector.hpp"

class Rays_receiver: public Permanent_Connector{
public:

Rays_receiver(unsigned short port_, sf::IpAddress remote_dev_ip_, sf::Clock& clock_);
void update() override;
void set_vectors_list(std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>>* vectors_list_);

private:
    std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>>* vectors_list = {};

    bool try_to_exchange_time();

    sf::Clock& clock;
    sf::Time time_limit = sf::milliseconds(500000000);
};


#endif //MAGISTERKA_RAYS_RECEIVER_HPP
