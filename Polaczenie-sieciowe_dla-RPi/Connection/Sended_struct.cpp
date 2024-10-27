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


sf::Packet& operator <<(sf::Packet& packet, Image_and_number& image_and_number){
    std::string str = std::string(image_and_number.img_vec.begin(), image_and_number.img_vec.end());

    return packet <<
    image_and_number.is_compressed <<
    image_and_number.col <<
    image_and_number.row <<
    image_and_number.type <<
    str <<
    image_and_number.image_number;
}


//sf::Packet& operator <<(sf::Packet& packet, const std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>>& axes_ratio){
//    packet.clear();
//
//    packet << static_cast<sf::Uint16>(axes_ratio.size());
//
//    for(auto& tuple: axes_ratio){
////        auto& begin_vec = std::get<0>(tuple);
////        auto& begin_end = std::get<1>(tuple);
////        auto& begin_centroid = std::get<2>(tuple);
//
//        std::vector<cv::Vec3d> tuple_as_vec = {std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple)};
//
//        for(auto& element_ratios: tuple_as_vec){
//            for(int i = 0; i < 3; i++){  // begin, end, centroids
//                packet << element_ratios[i];
//            }
//        }
//    }
//
//    return packet;
//};
//
//sf::Packet& operator >>(sf::Packet& packet, std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>>& axes_ratio){
//    sf::Uint16 size;
//    packet >> size;
//
//    axes_ratio.clear();
//    for(sf::Uint16 i = 0; i < size; i++){  // begin, end, centroids
//        cv::Vec3d new_element_vec[3];
//
//        for(auto & j : new_element_vec){
//            double axis_x, axis_y, axis_z;
//            packet >> axis_x >> axis_y >> axis_z;
//
//            j = cv::Vec3d(axis_x, axis_y, axis_z);
//        }
//
//        axes_ratio.emplace_back(new_element_vec[0], new_element_vec[1], new_element_vec[2]);
//    }
//    return packet;
//};
sf::Packet& operator <<(sf::Packet& packet, const std::vector<std::tuple<cv::Vec2d, cv::Vec2d, cv::Vec2d>>& axes_ratio){
    packet.clear();

    packet << static_cast<sf::Uint16>(axes_ratio.size());

    for(auto& tuple: axes_ratio){
//        auto& begin_vec = std::get<0>(tuple);
//        auto& begin_end = std::get<1>(tuple);
//        auto& begin_centroid = std::get<2>(tuple);

        std::vector<cv::Vec2d> tuple_as_vec = {std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple)};

        for(auto& element_ratios: tuple_as_vec){
            for(int i = 0; i < 2; i++){
                packet << element_ratios[i];
            }
        }
    }

    return packet;
};

sf::Packet& operator >>(sf::Packet& packet, std::vector<std::tuple<cv::Vec2d, cv::Vec2d, cv::Vec2d>>& axes_ratio){
    sf::Uint16 size;
    packet >> size;

    axes_ratio.clear();
    for(sf::Uint16 i = 0; i < size; i++){  // begin, end, centroids
        cv::Vec2d new_element_vec[3];

        for(auto & j : new_element_vec){
            double axis_x, axis_y, axis_z;
            packet >> axis_x >> axis_y;

            j = cv::Vec2d(axis_x, axis_y);
        }

        axes_ratio.emplace_back(new_element_vec[0], new_element_vec[1], new_element_vec[2]);
    }
    return packet;
};
