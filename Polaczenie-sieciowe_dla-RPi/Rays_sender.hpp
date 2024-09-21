//
// Created by wpr on 26.08.24.
//

#ifndef MAGISTERKA_RAYS_SENDER_HPP
#define MAGISTERKA_RAYS_SENDER_HPP

#include <opencv2/opencv.hpp>

#include "Pernament_Connector.hpp"

class Rays_sender: public Permanent_Connector{
public:
    Rays_sender(unsigned short port_, sf::IpAddress remote_dev_ip_, sf::Clock& clock_);
    void update() override;
    void set_vectors_list_ptr(std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>>* vectors_list_);
private:
    bool try_to_exchange_time();

    std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>>* vectors_list_ptr = nullptr;

    sf::Clock& clock;

    sf::Time time_limit_exchange_time_operation = sf::milliseconds(5000000000);
    sf::Time time_limit_retransfer = sf::milliseconds(500);
};


#endif //MAGISTERKA_RAYS_SENDER_HPP
