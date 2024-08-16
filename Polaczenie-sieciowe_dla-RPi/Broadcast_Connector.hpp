//
// Created by wr on 11/21/22.
//

#ifndef INZYNIERKA_BROADCAST_CONNECTOR_HPP
#define INZYNIERKA_BROADCAST_CONNECTOR_HPP


#include <SFML/Network.hpp>
#include <iostream>
#include <cstdlib>
#include <string>

#include "Time_Object.hpp"


class Broadcast_Connector: public Time_Object{
public:
    enum class b_connector_mode{
        primary,
        broadcast_ip_and_waiting_to_response,
        send_response,
        three_response_sended,
        bind_error
    };

    explicit Broadcast_Connector(unsigned short port_);

    b_connector_mode start_connection();
    void update() override;
    b_connector_mode get_mode();
    sf::IpAddress get_remote_ip();

private:
    unsigned short port = 0;
    sf::UdpSocket udp_socket;

    b_connector_mode mode = b_connector_mode::primary;

    sf::IpAddress remote_dev_ip;
};


#endif //INZYNIERKA_BROADCAST_CONNECTOR_HPP
