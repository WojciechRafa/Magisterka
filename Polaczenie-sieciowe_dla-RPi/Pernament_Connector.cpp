//
// Created by wr on 11/21/22.
//

#include "Pernament_Connector.hpp"
#include "main_functions.hpp"

Pernament_Connector::Pernament_Connector(unsigned short port_, sf::IpAddress remote_dev_ip_)

{
    set_port_and_remote_ip(port_, remote_dev_ip_);
 }

void Pernament_Connector::set_port_and_remote_ip(unsigned short port_, sf::IpAddress remote_dev_ip_) {
    port = port_;
    remote_dev_ip = remote_dev_ip_;
}

void Pernament_Connector::update() {
    if (mode == Pernament_Connector::p_connector_mode::establish_connection) {
        std::string ip_str = remote_dev_ip.toString();
        auto status = connect(remote_dev_ip, port);
        write_comunicate_sockte_status(status);

        if (status == sf::Socket::Status::Done) {
            std::cout << "TCP - Procedura nawiązywania kontaktu zakończona" << std::endl;
            mode = p_connector_mode::pernament_communication;
        }
    }
    last_update_time = clock.getElapsedTime().asMicroseconds();
}

Pernament_Connector::p_connector_mode Pernament_Connector::get_mode() {
    return mode;
}
