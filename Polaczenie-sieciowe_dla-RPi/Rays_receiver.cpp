//
// Created by wpr on 24.08.24.
//

#include "Rays_receiver.hpp"
#include "sended_struct.hpp"

Rays_receiver::Rays_receiver(unsigned short port_, sf::IpAddress remote_dev_ip_) :
Pernament_Connector(port_, remote_dev_ip_) {
    update_period_microseconds = 500000;
}

void Rays_receiver::update() {
    if(mode == Pernament_Connector::p_connector_mode::establish_connection){
        Pernament_Connector::update();
        if(get_mode() == Pernament_Connector::p_connector_mode::pernament_communication){
            update_period_microseconds = 50000;
        }
        // czas jest aktualziowany w Pernament_Connector::update();
    }else{
        // odbieranie
        sf::Packet received_packet;
        if(receive_n_time(received_packet)){
            vectors_list->clear();
            received_packet >> *vectors_list;
        }

        last_update_time = clock.getElapsedTime().asMicroseconds();
    }
}

bool Rays_receiver::receive_n_time(sf::Packet &received_packet) {
    setBlocking(false);

    bool was_any_good_packet = false;
    sf::Packet local_packet;

    for(int i = 0; i < max_number_of_receive_check; i++){
        auto status = receive(local_packet);

        if(status == sf::Socket::Done){
            if(not local_packet.endOfPacket()) {
                was_any_good_packet = true;
                received_packet = local_packet;
            }else{
                break;
            }

        }else{
            break;
        }
    }
    setBlocking(true);
    return was_any_good_packet;
}

void Rays_receiver::set_vectors_list(
        std::shared_ptr<std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>>> vectors_list_) {
    vectors_list = std::move(vectors_list_);
}
