//
// Created by wr on 11/21/22.
//

#ifndef INZYNIERKA_PERNAMENT_CONNECTOR_HPP
#define INZYNIERKA_PERNAMENT_CONNECTOR_HPP


#include <SFML/Network.hpp>
#include <iostream>
#include <cstdlib>
#include <string>

#include "Time_Object.hpp"


class Pernament_Connector: public sf::TcpSocket, public Time_Object{
public:
    enum class p_connector_mode{
        establish_connection,
        pernament_communication
    };

    Pernament_Connector(unsigned short port_, sf::IpAddress remote_dev_ip_);
    Pernament_Connector() = default;

    void set_port_and_remote_ip(unsigned short port_, sf::IpAddress remote_dev_ip_);

    void update() override ;
    p_connector_mode get_mode();
private:
    unsigned short port = 0;
    sf::IpAddress remote_dev_ip;

protected:
    p_connector_mode mode = p_connector_mode::establish_connection;
};

#endif //INZYNIERKA_PERNAMENT_CONNECTOR_HPP
