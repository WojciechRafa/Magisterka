//
// Created by wpr on 24.08.24.
//

#include "Rays_receiver.hpp"
#include "sended_struct.hpp"

Rays_receiver::Rays_receiver(unsigned short port_, sf::IpAddress remote_dev_ip_, Rays_intersection_analyzer* rays_intersection_analyzer):
    Rays_source(rays_intersection_analyzer),
    Permanent_Connector(port_, remote_dev_ip_){
    update_period_microseconds = 500000;
}

void Rays_receiver::update() {
    if(mode == Permanent_Connector::p_connector_mode::establish_connection){
        Permanent_Connector::update();
        if(get_mode() == Permanent_Connector::p_connector_mode::permanent_communication){
            update_period_microseconds = 50000; // TODO
            bool exchange_time_was_correct =  try_to_exchange_time();
            if(not exchange_time_was_correct){
                std::cout<<"Time exchange incorrect\n";
                disconnect();
            }else{
                setBlocking(true);
                std::cout<<"Time exchange correct\n";
            }
        }
        // czas jest aktualziowany w Permanent_Connector::update();
    }else if(mode == Permanent_Connector::p_connector_mode::permanent_communication) {
        // odbieranie
        sf::Packet received_packet;
        if(receive_n_time(received_packet, 10)){
            sf::Uint16 vector_size;
            sf::Int64 rays_time;
            std::vector<std::tuple<cv::Vec2d, cv::Vec2d, cv::Vec2d>> rays_data;

            read_packet(rays_time, vector_size, rays_data, received_packet);
            if(vector_size > 0 and rays_intersection_analyzer_ptr != nullptr){
                auto new_frame_parameter = std::make_unique<Frame_parameters>(rays_time, this, rays_data);
                rays_intersection_analyzer_ptr->add_projection(std::move(new_frame_parameter));
            }

//            std::cout<<"Rozmiar pakietu " << received_packet.getDataSize() << "\nRozmiar wektora " << vectors_list->bb_size() << std::endl;

        }
        std::cout<<"Data recive\n";
        last_update_time = clock.getElapsedTime().asMicroseconds();
    }
}

bool Rays_receiver::try_to_exchange_time() {
    sf::Packet sended_packet;
    auto sent_time = clock.getElapsedTime();
    sended_packet << sent_time.asMicroseconds();

    auto status = send(sended_packet);

    if(status != sf::Socket::Done){
        return false;
    }
    sf::Time begin_time = clock.getElapsedTime();
    while (clock.getElapsedTime() - begin_time < time_limit) {
        sf::Packet received_packet;

        if(receive_n_time(received_packet, 10)){
            sf::Int64 received_time_int;
            received_packet >> received_time_int;

            auto transfer_time = clock.getElapsedTime() - sent_time;
            return (sent_time.asMicroseconds() == received_time_int and transfer_time < time_limit);
        }

    }
    return false;
}
