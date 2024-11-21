//
// Created by wpr on 24.08.24.
//

#include "Rays_receiver.hpp"
#include "sended_struct.hpp"
#include "../main_functions.hpp"

Rays_receiver::Rays_receiver(unsigned short port_, sf::IpAddress remote_dev_ip_, Rays_intersection_analyzer* rays_intersection_analyzer):
    Rays_source(rays_intersection_analyzer, Configs::computers_enum::hp),
    Permanent_Connector(port_, remote_dev_ip_){

    auto remote_hw = Configs::remote_computer;
    auto remote_hw_folder_name = Configs::hw_folders_name[remote_hw];
    std::string main_folder = "../Hw_params/" + remote_hw_folder_name;
    internal_matrix = load_camera_matrix(main_folder + "/Camera_internal_parameters.csv");
    external_matrix = load_camera_matrix(main_folder + "/Camera_external_parameters.csv");
}

void Rays_receiver::update() {
    if(mode == Permanent_Connector::p_connector_mode::establish_connection){
        Permanent_Connector::update();
        if(get_mode() == Permanent_Connector::p_connector_mode::permanent_communication){
            update_period = sf::milliseconds(50); // TODO
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
            sf::Time rays_time;
            std::vector<std::tuple<cv::Vec2d, cv::Vec2d, cv::Vec2d>> rays_data;

            read_packet(rays_time, vector_size, rays_data, received_packet);
            if(vector_size > 0 and rays_intersection_analyzer_ptr != nullptr){
                auto new_frame_parameter = std::make_shared<Frame_parameters>(rays_time, this, rays_data);
                rays_intersection_analyzer_ptr->add_projection(new_frame_parameter);
            }
        }
        last_update_time = clock.getElapsedTime();
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
