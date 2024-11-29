//
// Created by wpr on 24.08.24.
//

#ifndef MAGISTERKA_PARAMETERS_RECEIVER_HPP
#define MAGISTERKA_PARAMETERS_RECEIVER_HPP

#include <opencv2/opencv.hpp>

#include "Permanent_Connector.hpp"
#include "../Rays_source.hpp"

class Parameters_receiver: public Permanent_Connector, public Rays_source{
public:

Parameters_receiver(unsigned short port_, sf::IpAddress remote_dev_ip_, Rays_intersection_analyzer* rays_intersection_analyzer);
void update() override;
private:

    bool try_to_exchange_time();

    sf::Time time_limit = sf::milliseconds(500000000);
};


#endif //MAGISTERKA_PARAMETERS_RECEIVER_HPP
