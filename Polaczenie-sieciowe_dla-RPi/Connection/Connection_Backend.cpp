//
// Created by wr on 12/17/22.
//

#include "Connection_Backend.hpp"

Connection_Backend::Connection_Backend(unsigned short port_,
                                       std::vector<Custom_Data_IO_Window::message>& message_list_sended_,
                                       std::vector<Custom_Data_IO_Window::message>& message_list_recived_,
                                       sf::Clock& clock_
                                       ):
port(port_),
message_list_sended(message_list_sended_),
message_list_recived(message_list_recived_),
clock(clock_)
{
}

Connection_Backend::Connection_Backend(unsigned short port_,
                                       sf::Clock& clock_,
                                       Rays_intersection_analyzer* rays_intersection_analyzer_ptr_):
        port(port_),
        clock(clock_),
        rays_intersection_analyzer_ptr(rays_intersection_analyzer_ptr_){}

const sf::Image* Connection_Backend::get_image() {
    if(image_receiver != nullptr){
        return image_receiver->get_image();
    }
    return nullptr;
}

Connection_Backend::Connection_State Connection_Backend::get_connection_state() {
    return connection_state;
}

bool Connection_Backend::start_looking_for_another_device() {
    if(connection_state == Connection_State::primary){
        broadcast_connector = std::make_unique<Broadcast_Connector>(port);

        if (broadcast_connector->start_connection() != Broadcast_Connector::b_connector_mode::bind_error) {
            std::cout << "Zbindowano \n";
            connection_state = Connection_State::broadcast;
            return true;
        }else if(broadcast_connector->start_connection() != Broadcast_Connector::b_connector_mode::bind_error){
            std::cout << "Nie można zbindować \n";
            connection_state = Connection_State::bind_error;

            throw std::exception();
        }
    }else if(connection_state != Connection_State::broadcast){
        return false;
    }
    return false;
}

bool Connection_Backend::start_connection_camera_view() {
    if(connection_state == Connection_State::another_IP_knowed or
       connection_state == Connection_State::only_custom_data_work
    ){
        image_receiver = std::make_unique<Image_Receiver>(port, remote_ip);

        if(remote_ip != sf::IpAddress::None) {

            if(connection_state == Connection_State::another_IP_knowed)
                connection_state = Connection_State::camera_view_connect_establishment_custom_data_not_work;
            else{
                connection_state = Connection_State::camera_view_connect_establishment_custom_data_work;
            }

            return true;
        }else{
            std::cout<<"Adres IP urządzenia zdalnego jest nieznany \n";
            connection_state = Connection_State::incorect_connect_request_IP_error;
            return false;
        }
    }
    return false;
}

bool Connection_Backend::start_connection_custom_data() {
    if(connection_state == Connection_State::another_IP_knowed or
       connection_state == Connection_State::only_camera_view_work
       )
    {
         custom_data_io = std::make_unique<Custom_Data_IO>(port + 1, remote_ip);
         for(auto& message: message_list_sended){
             custom_data_io->add_sent_message(message);
         }
        for(auto& message: message_list_recived){
            custom_data_io->add_received_message(message);
        }
         if(remote_ip != sf::IpAddress::None) {
            if(connection_state == Connection_State::another_IP_knowed)
                connection_state = Connection_State::custom_data_connect_establishment_camera_view_not_work;
            else{
                connection_state = Connection_State::custom_data_connect_establishment_camera_view_work;
            }

            return true;
        }else{
            std::cout<<"Adres IP urządzenia zdalnego jest nieznany \n";
            connection_state = Connection_State::incorect_connect_request_IP_error;
            return false;
        }
    }
    return false;
}

bool Connection_Backend::start_connection_axes_ratio() {
    if(connection_state == Connection_State::another_IP_knowed or
       connection_state == Connection_State::only_custom_data_work){
        rays_receiver = std::make_unique<Rays_receiver>(port, remote_ip, rays_intersection_analyzer_ptr);

        if(remote_ip != sf::IpAddress::None) {

            connection_state = Connection_State::axes_ratio_work;
//            if(connection_state == Connection_State::another_IP_knowed)
//                connection_state = Connection_State::camera_view_connect_establishment_custom_data_not_work;
//            else{
//                connection_state = Connection_State::camera_view_connect_establishment_custom_data_work;
//            }

            return true;
        }else{
            std::cout<<"Adres IP urządzenia zdalnego jest nieznany \n";
            connection_state = Connection_State::incorect_connect_request_IP_error;
            return false;
        }
    }
    return false;
}

void Connection_Backend::update_st() {
    if(connection_state == Connection_State::broadcast){
        if (Broadcast_Connector::b_connector_mode::three_response_sended == broadcast_connector->get_mode()) {
            remote_ip = broadcast_connector->get_remote_ip();
            std::cout << "UDP - Procedura nawiązywania kontaktu zakończona" << std::endl;

            connection_state = Connection_State::another_IP_knowed;
            broadcast_connector = nullptr;
        }
    }else if(connection_state == Connection_State::camera_view_connect_establishment_custom_data_not_work or
             connection_state == Connection_State::camera_view_connect_establishment_custom_data_work
    ) {
        if(image_receiver == nullptr){
            throw std::exception();
        }

            auto status = image_receiver->get_mode();

            if (status == Permanent_Connector::p_connector_mode::permanent_communication){
                std::cout <<" Camera View - Procedura nawiązywania kontaktu zakończona" << std::endl;

                if(connection_state == Connection_State::camera_view_connect_establishment_custom_data_work){
                    connection_state = Connection_State::camera_view_and_custom_data_work;
                }else{
                    connection_state = Connection_State::only_camera_view_work;
                }
            }
    }else if(connection_state == Connection_State::custom_data_connect_establishment_camera_view_not_work or
             connection_state == Connection_State::custom_data_connect_establishment_camera_view_work) {
        if(custom_data_io == nullptr){
            throw std::exception();
        }

        auto status = custom_data_io->get_mode();

        if (status == Permanent_Connector::p_connector_mode::permanent_communication){
            std::cout <<" Custom Data - Procedura nawiązywania kontaktu zakończona" << std::endl;

            if(connection_state == Connection_State::custom_data_connect_establishment_camera_view_work){
                connection_state = Connection_State::camera_view_and_custom_data_work;
            }else{
                connection_state = Connection_State::only_custom_data_work;
            }
        }
    }else if(   connection_state == Connection_State::axes_ratio_establishment or
                connection_state == Connection_State::axes_ratio_work
            ){
        auto status = rays_receiver->get_mode();

        if (status == Permanent_Connector::p_connector_mode::permanent_communication){

            connection_state = Connection_State::axes_ratio_work;

        }
    }
}

std::vector<Custom_Data_IO_Window::message> Connection_Backend::get_recived_message_list(){
    if(custom_data_io == nullptr){
        return {};
    }else{
        return custom_data_io->get_recived_message_list();
    }
}

bool Connection_Backend::is_camera_view_work() {
    return connection_state == Connection_State::camera_view_connect_establishment_custom_data_work or
           connection_state == Connection_State::camera_view_connect_establishment_custom_data_not_work or
           connection_state == Connection_State::custom_data_connect_establishment_camera_view_work or
           connection_state == Connection_State::only_camera_view_work or
           connection_state == Connection_State::camera_view_and_custom_data_work;
}

bool Connection_Backend::is_custom_data_work() {
    return connection_state == Connection_State::camera_view_connect_establishment_custom_data_work or
            connection_state == Connection_State::custom_data_connect_establishment_camera_view_work or
            connection_state == Connection_State::custom_data_connect_establishment_camera_view_not_work or
            connection_state == Connection_State::only_custom_data_work or
            connection_state == Connection_State::camera_view_and_custom_data_work;
}

bool Connection_Backend::is_custom_rays_receiver() {
    return connection_state == Connection_State::axes_ratio_establishment or
           connection_state == Connection_State::axes_ratio_work;
}

void Connection_Backend::set_camera_view_mode(Image_Receiver::Sender_Mode mode) {
    if(image_receiver != nullptr){
        image_receiver->set_camera_view_mode(mode);
    }
}


