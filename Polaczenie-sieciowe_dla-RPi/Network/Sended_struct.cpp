#include "Sended_struct.hpp"

#include <iostream>

sf::Packet& operator<<(sf::Packet& packet, const Control_message& directions)
{
    return packet << directions.left_engine << directions.right_engine;
}

sf::Packet& operator>>(sf::Packet& packet, Control_message& directions)
{
    return packet >> directions.left_engine >> directions.right_engine;
}



sf::Packet& operator <<(sf::Packet& packet, const sf::IpAddress& ip){
    return packet << ip.toInteger();
}

sf::IpAddress get_ip(sf::Packet packet){
    sf::Uint32 ip_int = 0;
    packet >> ip_int;
    return sf::IpAddress(ip_int);
}

sf::Packet& operator <<(sf::Packet& packet, const Double_ip_message& ips){
    return packet << ips.sender.toInteger() << ips.receiver.toInteger();
}

sf::Packet& operator >>(sf::Packet& packet, Double_ip_message& ips){
    sf::Uint32 ips_int_sender, ips_int_receiver;
    packet >> ips_int_sender >> ips_int_receiver;
    ips.sender = sf::IpAddress(ips_int_sender);
    ips.receiver = sf::IpAddress(ips_int_receiver);
    return packet;
}


sf::Packet& operator <<(sf::Packet& packet, Image_and_number& image_and_number)
{
    std::string str = std::string(image_and_number.img_vec.begin(), image_and_number.img_vec.end());

    return packet <<
    image_and_number.is_compressed <<
    image_and_number.col <<
    image_and_number.row <<
    image_and_number.type <<
    str <<
    image_and_number.image_number;
}

void write_packet(sf::Time& time_main,
                  std::vector<std::tuple<cv::Vec2d, cv::Vec2d, cv::Vec2d>>& data,
                  sf::Packet& packet){
    packet.clear();

    packet << time_main.asMicroseconds();
    packet << static_cast<sf::Uint16>(data.size());

    for(auto& tuple: data){
        std::vector<cv::Vec2d> tuple_as_vec = {std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple)};

        for(auto& element_ratios: tuple_as_vec){
            for(int i = 0; i < 2; i++){
                packet << element_ratios[i];
            }
        }
    }
}