//
// Created by wpr on 26.08.24.
//

#include "Parameter_sender.hpp"
#include "Sended_struct.hpp"

Parameter_sender::Parameter_sender(unsigned short port_, sf::IpAddress remote_dev_ip_) : Permanent_Connector(
                                                                                                    port_,
                                                                                                    remote_dev_ip_)
                                                                                                    {
    // update_period_microseconds = 500000;
    update_period_microseconds = 50000;
}

void Parameter_sender::update() {
    std::cout<<"\nParameter_sender::update() time:";
    auto time_begin = clock.getElapsedTime().asMicroseconds();
    if(mode == Permanent_Connector::p_connector_mode::establish_connection){
        if(const auto status = tcp_listener.accept(*this); status == sf::Socket::Status::Done) {
            setBlocking(true);
            if(try_to_exchange_time())
            {
                mode = p_connector_mode::permanent_communication;
                std::cout<<"Time exchange correct\n";
                std::cout<<"Connection done\n";
            }
            setBlocking(false);
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
        std::cout<<"Sent data, result "<< status << "\n";
    }
    last_update_time = clock.getElapsedTime().asMicroseconds();
    std::cout<<last_update_time - time_begin<< " microseconds \n";
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

void Parameter_sender::set_update_period(sf::Int64 update_period_microsecond_) {
    update_period_microseconds = update_period_microsecond_;
}
