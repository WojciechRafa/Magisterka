//
// Created by wpr on 24.08.24.
//

#include "Rays_receiver.hpp"
#include "sended_struct.hpp"

Rays_receiver::Rays_receiver(unsigned short port_, sf::IpAddress remote_dev_ip_, sf::Clock& clock_) :
        Permanent_Connector(port_, remote_dev_ip_),
        clock(clock_){
    update_period_microseconds = 500000;
}

void Rays_receiver::update() {
    if(mode == Permanent_Connector::p_connector_mode::establish_connection){
        Permanent_Connector::update();
        if(get_mode() == Permanent_Connector::p_connector_mode::permanent_communication){
            update_period_microseconds = 50000;
            bool exchange_time_was_correct =  try_to_exchange_time();
            if(not exchange_time_was_correct){
                disconnect();
            }
        }
        // czas jest aktualziowany w Permanent_Connector::update();
    }else{
        // odbieranie
        sf::Packet received_packet;
        if(receive_n_time(received_packet)){
            vectors_list->clear();
            received_packet >> *vectors_list;

            std::cout<<"Rozmiar pakietu " << received_packet.getDataSize() << "\nRozmiar wektora " << vectors_list->size() << std::endl;

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

void Rays_receiver::set_vectors_list(std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>>* vectors_list_) {
    vectors_list = vectors_list_;
}

bool Rays_receiver::try_to_exchange_time() {
    setBlocking(false);
    sf::Packet sended_packet;
    auto sent_time = clock.getElapsedTime();
    sended_packet << sent_time.asMicroseconds();

    auto status = send(sended_packet);

    if(status != sf::Socket::Done){
        return false;
    }

    while (clock.getElapsedTime() < time_limit) {
        sf::Packet received_packet;
        status = receive(received_packet);
        if (status == sf::Socket::Done) {
            if (not received_packet.endOfPacket()) {
                continue;
            } else {
                std::cout << "End of packet \n";
                sf::Int64 received_time_int;
                received_packet >> received_time_int;

                auto transfer_time = clock.getElapsedTime() - sent_time;
                return (sent_time.asMicroseconds() == received_time_int and transfer_time < time_limit);
            }
        }

    }

    return false;
}
