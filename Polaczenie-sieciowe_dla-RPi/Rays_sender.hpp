//
// Created by wpr on 26.08.24.
//

#ifndef MAGISTERKA_RAYS_SENDER_HPP
#define MAGISTERKA_RAYS_SENDER_HPP

#include <opencv2/opencv.hpp>

#include "Pernament_Connector.hpp"

class Rays_sender final : public Pernament_Connector{
public:
    Rays_sender(unsigned short port_, sf::IpAddress remote_dev_ip_);
    void update() override;
    void set_vectors_list_ptr(std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>>* vectors_list_);
private:
    std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>>* vectors_list_ptr = nullptr;
};


#endif //MAGISTERKA_RAYS_SENDER_HPP
