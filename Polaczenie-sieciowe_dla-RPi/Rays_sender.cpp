//
// Created by wpr on 26.08.24.
//

#include "Rays_sender.hpp"
#include "Sended_struct.hpp"

Rays_sender::Rays_sender(unsigned short port_, sf::IpAddress remote_dev_ip_) : Permanent_Connector(port_,
                                                                                                   remote_dev_ip_) {
    update_period_microseconds = 500000;
}

void Rays_sender::update() {
    if(mode == Permanent_Connector::p_connector_mode::establish_connection){
        Permanent_Connector::update();

        auto status = tcp_listener.accept(*this);
        if(status == sf::Socket::Status::Done) {

        }

    }else{
        if(vectors_list_ptr == nullptr){
            std::cerr<<"vectors_list_ptr is nullptr \n";
        }

        // nadawanie
        sf::Packet sended_packet;

        sended_packet << vectors_list_ptr;

        auto status = send(sended_packet);

        last_update_time = clock.getElapsedTime().asMicroseconds();
    }
}

void Rays_sender::set_vectors_list_ptr(std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>>* vectors_list_) {
    vectors_list_ptr = vectors_list_;
}

bool Rays_sender::try_to_exchange_time() {



    return false;
}
