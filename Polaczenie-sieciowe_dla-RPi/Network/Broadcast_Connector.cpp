//
// Created by wr on 11/21/22.
//

#include "Broadcast_Connector.hpp"

#include "sended_struct.hpp"

Broadcast_Connector::Broadcast_Connector(unsigned short port_):
    port(port_){
    udp_socket.setBlocking(false);

    update_period_microseconds = 500000;
}
void Broadcast_Connector::update() {
    if (mode == b_connector_mode::broadcast_ip_and_waiting_to_response) {
        // Nadawanie
        sf::Packet packet_sended;
        packet_sended << sf::IpAddress::getLocalAddress();

        udp_socket.send(packet_sended, sf::IpAddress::Broadcast, port);

        static int itr = 0;
        std::cout << "Wysłano komunikat "<< itr << std::endl;

        itr ++;

        // Odbieranie
        sf::Packet packet_received;
        sf::IpAddress sender_ip;
        if (udp_socket.receive(packet_received, sender_ip, port) == sf::Socket::Done) {
            Double_ip_message ip_message_received;
            packet_received >> ip_message_received;

            auto r = ip_message_received.receiver.toString();
            auto s = ip_message_received.sender.toString();

            if (ip_message_received.receiver == sf::IpAddress::getLocalAddress() and
                ip_message_received.sender == sender_ip) {
                std::cout << "Otrzymano wiadomość zwrotna " << std::endl;
                remote_dev_ip = sender_ip;
                mode = b_connector_mode::send_response;
            }
        }
    } else if(mode == b_connector_mode::send_response) {
        std::cout << "Wysylanie odpowiedzi" << std::endl;

        Double_ip_message ip_message_sended = {
                sf::IpAddress::getLocalAddress(),
                remote_dev_ip
        };

        sf::Packet sended_packet;
        sended_packet << ip_message_sended;

        std::cout << "Nadajnik " <<ip_message_sended.sender << std::endl;
        std::cout << "Odbiornik "<<ip_message_sended.receiver << std::endl;

        udp_socket.send(sended_packet, remote_dev_ip, port);
        std::cout << "Wysłano potwierdzenie  " << std::endl;

        static short nr_of_sended_response = 0;
        nr_of_sended_response++;
        if(nr_of_sended_response >= 3)
            mode = b_connector_mode::three_response_sended;
    }

    last_update_time = clock.getElapsedTime().asMicroseconds();
}


sf::IpAddress Broadcast_Connector::get_remote_ip(){
    return remote_dev_ip;
}

Broadcast_Connector::b_connector_mode Broadcast_Connector::get_mode() {
    return mode;
}

Broadcast_Connector::b_connector_mode Broadcast_Connector::start_connection() {
    auto bind_status = udp_socket.bind(port);
    if(bind_status != sf::Socket::Status::Done){
        mode = b_connector_mode::bind_error;
    }

    if(mode == b_connector_mode::primary){
        mode = b_connector_mode::broadcast_ip_and_waiting_to_response;
    }

    return mode;
}


