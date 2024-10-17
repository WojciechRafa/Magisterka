//
// Created by wpr on 26.08.24.
//

#include "Parameter_sender.hpp"
#include "Sended_struct.hpp"

Parameter_sender::Parameter_sender(unsigned short port_, sf::IpAddress remote_dev_ip_, sf::Clock& clock_) : Permanent_Connector(
                                                                                                    port_,
                                                                                                    remote_dev_ip_),
                                                                                                            clock(clock_)
                                                                                                    {
    update_period_microseconds = 500000;
}

void Parameter_sender::update() {
    if(mode == Permanent_Connector::p_connector_mode::establish_connection){
        Permanent_Connector::update();

        auto status = tcp_listener.accept(*this);
        if(status == sf::Socket::Status::Done) {
            bool exchange_time_was_correct =  try_to_exchange_time();
            std::cout<<"Connection done\n";
        }

    }else{
        if(objets_parameters_list_ptr == nullptr){
            std::cerr<<"objets_parameters_list_ptr is nullptr \n";
        }

        // nadawanie
        sf::Packet sended_packet;

        sended_packet << *objets_parameters_list_ptr;
        std::cout<<"Rozmiar danych " << sended_packet.getDataSize()<<"\n";
        auto status = send(sended_packet);

        last_update_time = clock.getElapsedTime().asMicroseconds();
    }
}

//void Parameter_sender::set_vectors_list_ptr(std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>>* vectors_list_) {
//    vectors_list_ptr = vectors_list_;
//}

bool Parameter_sender::try_to_exchange_time() {
    sf::Packet received_packet;

    sf::Time begin_time = clock.getElapsedTime();
    while (clock.getElapsedTime() - begin_time < time_limit_exchange_time_operation) {

        if(receive_n_time(received_packet, 10)){
            sf::Int64 master_time;
            received_packet >> master_time;

            auto time_diff = clock.getElapsedTime() - sf::microseconds(master_time);
            auto status = send(received_packet);

            return status == sf::Socket::Done and time_diff < time_limit_exchange_time_operation;
        }
    }
    return false;
}

void
Parameter_sender::set_objets_parameters_list_ptr(std::vector<std::tuple<cv::Vec2d, cv::Vec2d, cv::Vec2d>> *objets_parameter_list_ptr_) {
    objets_parameters_list_ptr = objets_parameter_list_ptr_;
}
