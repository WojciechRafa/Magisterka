#pragma once

#include <SFML/Network.hpp>
#include <iostream>
#include <opencv2/core/hal/interface.h>
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
    int  cols;
    int  rows;
    int  type;
    std::vector<u_char> img_vec;
    sf::Uint32 image_number;
};



sf::Packet& operator <<(sf::Packet& packet, const Control_message& directions);
sf::Packet& operator >>(sf::Packet& packet, Control_message& directions);

sf::Packet& operator <<(sf::Packet& packet, const Double_ip_message& ips);
sf::Packet& operator >>(sf::Packet& packet, Double_ip_message& ips);

sf::Packet& operator <<(sf::Packet& packet, const sf::IpAddress& ip);

sf::IpAddress get_ip(sf::Packet& packet);

sf::Packet& operator >>(sf::Packet& packet, Image_and_number& Img_and_num);
