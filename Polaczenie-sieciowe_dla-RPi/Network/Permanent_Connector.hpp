//
// Created by wr on 11/21/22.
//

#ifndef INZYNIERKA_PERNAMENT_CONNECTOR_HPP
#define INZYNIERKA_PERNAMENT_CONNECTOR_HPP


#include <SFML/Network.hpp>
#include <iostream>
#include <cstdlib>
#include <string>

#include "../Time_Object.hpp"


class Permanent_Connector: public sf::TcpSocket, public Time_Object{
public:
    enum class p_connector_mode{
        disconnected,
        establish_connection,
        permanent_communication
    };

    Permanent_Connector(unsigned short port_, sf::IpAddress remote_dev_ip_);
    Permanent_Connector() = default;

    void set_port_and_remote_ip(unsigned short port_, sf::IpAddress remote_dev_ip_);

    void update() override ;
    p_connector_mode get_mode();

    void disconnect();

    std::string get_name() override{return "Pernament Connectior";};
private:
    unsigned short port = 0;
    sf::IpAddress remote_dev_ip;

protected:
    bool receive_n_time(sf::Packet& received_packet, int max_number_of_receive_check);

    p_connector_mode mode = p_connector_mode::establish_connection;
};

#endif //INZYNIERKA_PERNAMENT_CONNECTOR_HPP
