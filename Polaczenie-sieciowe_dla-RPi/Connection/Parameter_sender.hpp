//
// Created by wpr on 26.08.24.
//

#ifndef MAGISTERKA_PARAMETER_SENDER_HPP
#define MAGISTERKA_PARAMETER_SENDER_HPP

#include <opencv2/opencv.hpp>

#include "Pernament_Connector.hpp"

class Parameter_sender: public Permanent_Connector{
public:
    Parameter_sender(unsigned short port_, sf::IpAddress remote_dev_ip_);
    void update() override;
//    void set_vectors_list_ptr(std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>>* vectors_list_);
    void set_objets_parameters_list_ptr(std::vector<std::tuple<cv::Vec2d, cv::Vec2d, cv::Vec2d>>* objets_parameter_list_ptr_);

//    std::string get_name() {return "Parameter_sender";}

    void set_update_period(sf::Int64 update_period_microsecond_);

//    bool need_update() override {
//        std::cout<<"Elo Parameter_sender need_update"<<std::endl;
//        return clock.getElapsedTime().asMicroseconds() - last_update_time > update_period_microseconds;
//    };
private:
    bool try_to_exchange_time();

//    std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>>* vectors_list_ptr = nullptr;
    std::vector<std::tuple<cv::Vec2d, cv::Vec2d, cv::Vec2d>>* objets_parameters_list_ptr = nullptr;

//    sf::Clock& clock;

    sf::Time time_limit_exchange_time_operation = sf::milliseconds(500000);
    sf::Time time_limit_retransfer = sf::milliseconds(500);
};


#endif //MAGISTERKA_PARAMETER_SENDER_HPP
