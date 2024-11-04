//
// Created by wpr on 26.08.24.
//

#ifndef MAGISTERKA_PARAMETER_SENDER_HPP
#define MAGISTERKA_PARAMETER_SENDER_HPP

#include <opencv2/opencv.hpp>

#include "Pernament_Connector.hpp"
#include "Sended_struct.hpp"

class Parameter_sender: public Permanent_Connector{
public:
    Parameter_sender(unsigned short port_, sf::IpAddress remote_dev_ip_);
    void update() override;
//    void set_sent_parameters_ptr(std::vector<std::tuple<cv::Vec2d, cv::Vec2d, cv::Vec2d>>* objets_parameter_list_ptr_);
    void set_sent_parameters_ptr(Sent_parameters* sent_parameters_ptr_);

    void set_update_period(sf::Int64 update_period_microsecond_);
private:
    bool try_to_exchange_time();

    Sent_parameters* sent_parameters_ptr = nullptr;

    sf::Time time_limit_exchange_time_operation = sf::milliseconds(500000);
    sf::Time time_limit_retransfer = sf::milliseconds(500);
};


#endif //MAGISTERKA_PARAMETER_SENDER_HPP
