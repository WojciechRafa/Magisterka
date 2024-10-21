#include "Pernament_Connector.hpp"

#include "Sended_struct.hpp"
#include "main_functions.hpp"

Permanent_Connector::Permanent_Connector(unsigned short port_, sf::IpAddress remote_dev_ip_):
	port(port_),
	remote_dev_ip(remote_dev_ip_){
		
	auto listen_status = tcp_listener.listen(port);
    
	std::cout<<"TCP - listener status: \n";
	write_comunicate_sockte_status(listen_status);
	
	setBlocking(false);
    tcp_listener.setBlocking(false);
}


Permanent_Connector::p_connector_mode Permanent_Connector::get_mode() {
    return mode;
}

void Permanent_Connector::update() {

    if(mode == p_connector_mode::establish_connection){
        auto status = tcp_listener.accept(*this);

        if(status == sf::Socket::Status::Done) {
            std::cout<<"TCP - Połączenie poprawne \n";
            mode = p_connector_mode::permanent_communication;
            setBlocking(true);
        }
    }else
        throw std::exception();

    last_update_time = clock.getElapsedTime().asMicroseconds();
}

void Permanent_Connector::disconnect() {
    std::cout << "Permanent connector is disconnected" << std::endl;
    TcpSocket::disconnect();
    mode = p_connector_mode::disconnected;
}

bool Permanent_Connector::receive_n_time(sf::Packet &received_packet, int max_number_of_receive_check) {
    setBlocking(false);

    bool was_any_good_packet = false;
    sf::Packet local_packet;

    for(int i = 0; i < max_number_of_receive_check; i++){
        auto status = receive(local_packet);

        if(status == sf::Socket::Done){
            if(not local_packet.endOfPacket()) {
                was_any_good_packet = true;
                received_packet = local_packet;
            }else{
                break;
            }

        }else{
            break;
        }
    }
    setBlocking(true);
    return was_any_good_packet;
}
