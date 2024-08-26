//
// Created by wpr on 24.08.24.
//

#ifndef MAGISTERKA_RAYS_RECEIVER_HPP
#define MAGISTERKA_RAYS_RECEIVER_HPP

#include <opencv2/opencv.hpp>

#include "Pernament_Connector.hpp"

class Rays_receiver: public Pernament_Connector{
public:
Rays_receiver(unsigned short port_, sf::IpAddress remote_dev_ip_);
void update() override;
void set_vectors_list(std::shared_ptr<std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>>> vectors_list_);
private:
    bool receive_n_time(sf::Packet& received_packet);
    const int max_number_of_receive_check = 10;

    std::shared_ptr<std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>>> vectors_list;
};


#endif //MAGISTERKA_RAYS_RECEIVER_HPP
