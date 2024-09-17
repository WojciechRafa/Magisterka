#pragma once

#include <SFML/Network.hpp>
#include <iostream>
#include <cstdlib>
#include <string>

#include "Time_Object.hpp"

class Permanent_Connector:public sf::TcpSocket, public Time_Object{
public:
    enum class p_connector_mode{
        establish_connection,
        pernament_communication
    };

	Permanent_Connector(unsigned short port_, sf::IpAddress remote_dev_ip_);

    Permanent_Connector::p_connector_mode get_mode();

    void update() override;
protected:
    p_connector_mode mode = p_connector_mode::establish_connection;
    sf::TcpListener tcp_listener;
private:
	unsigned short port;
	sf::IpAddress remote_dev_ip;
};
