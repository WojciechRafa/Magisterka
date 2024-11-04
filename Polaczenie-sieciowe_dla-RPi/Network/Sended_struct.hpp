#pragma once

#include <SFML/Network.hpp>
#include <opencv2/opencv.hpp>

struct Control_message{
    sf::Int16 left_engine;
    sf::Int16 right_engine;
};

struct Double_ip_message{
    sf::IpAddress sender;
    sf::IpAddress receiver;
};

struct Image_and_number{
    bool is_compressed;
    sf::Int32 col;
    sf::Int32 row;
    sf::Int32 type;
    std::vector<u_char> img_vec;
    sf::Uint32 image_number;
};

struct Sent_parameters{
    sf::Time main_time;
    std::vector<std::tuple<cv::Vec2d, cv::Vec2d, cv::Vec2d>> data;
};

sf::Packet& operator <<(sf::Packet& packet, const Control_message& directions);
sf::Packet& operator >>(sf::Packet& packet, Control_message& directions);

sf::Packet& operator <<(sf::Packet& packet, const Double_ip_message& ips);
sf::Packet& operator >>(sf::Packet& packet, Double_ip_message& ips);

sf::Packet& operator <<(sf::Packet& packet, Image_and_number& image_and_number);

sf::IpAddress get_ip(sf::Packet packet);

//sf::Packet& operator <<(sf::Packet& packet, const std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>>& axes_ratio);
//sf::Packet& operator >>(sf::Packet& packet, std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>>& axes_ratio);
//sf::Packet& operator <<(sf::Packet& packet, const std::vector<std::tuple<cv::Vec2d, cv::Vec2d, cv::Vec2d>>& axes_ratio);
//sf::Packet& operator >>(sf::Packet& packet, std::vector<std::tuple<cv::Vec2d, cv::Vec2d, cv::Vec2d>>& axes_ratio);

void write_packet(sf::Time& time_main,
                 std::vector<std::tuple<cv::Vec2d, cv::Vec2d, cv::Vec2d>>& data,
                 sf::Packet& packet);
