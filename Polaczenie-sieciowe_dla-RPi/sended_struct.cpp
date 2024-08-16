#include "sended_struct.hpp"

sf::Packet& operator <<(sf::Packet& packet, const Control_message& directions)
{
    return packet << directions.left_engine << directions.right_engine;
}

sf::Packet& operator >>(sf::Packet& packet, Control_message& directions)
{
    return packet >> directions.left_engine >> directions.right_engine;
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

sf::Packet& operator <<(sf::Packet& packet, const sf::IpAddress& ip){
    return packet << ip.toInteger();
}

sf::IpAddress get_ip(sf::Packet& packet){
    sf::Uint32 ip_int = 0;
    packet >> ip_int;
    return sf::IpAddress(ip_int);
}

sf::Packet& operator >>(sf::Packet& packet, Image_and_number& Img_and_num){
    // Obiekt pośredni
    std::string img_str;

    packet >>
    Img_and_num.is_compressed >>
    Img_and_num.cols >>
    Img_and_num.rows >>
    Img_and_num.type >>
    img_str>>
    Img_and_num.image_number;
    std::copy(img_str.begin(), img_str.end(), std::back_inserter(Img_and_num.img_vec));

    return packet;
}