//
// Created by wpr on 29.12.22.
//

#ifndef INZYNIERKA_CUSTOM_DATA_IO_HPP
#define INZYNIERKA_CUSTOM_DATA_IO_HPP

#include "Pernament_Connector.hpp"
#include "Custom_Data_IO_Window.hpp"

class Custom_Data_IO: public Pernament_Connector{
public:
    Custom_Data_IO(unsigned short port_, sf::IpAddress remote_dev_ip_);

    void update() override;
    void add_sent_message(const Custom_Data_IO_Window::message& message);
    void add_received_message(const Custom_Data_IO_Window::message& message);

    bool update_variable_by_name_int(const std::string& name, sf::Int32 value);
    bool update_variable_by_name_float(const std::string& name, float value);

    bool get_variable_by_name_int(const std::string& name, sf::Int32& value);
    bool get_variable_by_name_float(const std::string& name, float& value);

    bool get_variable_by_id_int(const std::string& name, sf::Int32& value);
    bool get_variable_by_id_float(const std::string& name, float& value);

    std::vector<Custom_Data_IO_Window::message>& get_recived_message_list();
private:
    std::vector<Custom_Data_IO_Window::message> sended_message_list;
    std::vector<Custom_Data_IO_Window::message> recived_message_list;

    Custom_Data_IO_Window::message* find_message_by_id_recived(sf::Int32 id);

    bool update_recived(sf::Packet& recived_packet);

    sf::Packet prepare_packet_to_send();

    bool receive_n_time(sf::Packet& received_packet);
    const int max_number_of_recived_check = 10;
};


#endif //INZYNIERKA_CUSTOM_DATA_IO_HPP
