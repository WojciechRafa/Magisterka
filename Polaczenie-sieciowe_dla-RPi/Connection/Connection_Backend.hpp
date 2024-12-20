//
// Created by wr on 12/17/22.
//

#ifndef INZYNIERKA_CONNECTION_BACKEND_HPP
#define INZYNIERKA_CONNECTION_BACKEND_HPP

#include "../Time_Object.hpp"
#include "../Network/Broadcast_Connector.hpp"
#include "../Network/Image_Receiver.hpp"
#include "../Network/Custom_Data_IO.hpp"
#include "../Network/Parameters_receiver.hpp"
#include "../Image_Analysis/Rays_intersection_analyzer.hpp"

#include <SFML/Network.hpp>
#include <opencv2/opencv.hpp>

class Connection_Backend{
public:
    explicit Connection_Backend(unsigned short port_,
                                std::vector<Custom_Data_IO_Window::message>& message_list_recived_,
                                std::vector<Custom_Data_IO_Window::message>& message_list_sended_);

    explicit Connection_Backend(unsigned short port_,
                                sf::Clock& clock_,
                                Rays_intersection_analyzer* rays_intersection_analyzer_ptr_);

    enum class Connection_State{
        primary,
        broadcast,
        bind_error,
        another_IP_knowed,
        incorect_connect_request_IP_error,
        camera_view_connect_establishment_custom_data_work,
        camera_view_connect_establishment_custom_data_not_work,
        custom_data_connect_establishment_camera_view_work,
        custom_data_connect_establishment_camera_view_not_work,
        only_camera_view_work,
        only_custom_data_work,
        camera_view_and_custom_data_work,
        axes_ratio_establishment,
        axes_ratio_work
    };


    // funkcje zwracają true przy powodzeniu
    bool start_looking_for_another_device();

    bool start_connection_camera_view();
    bool start_connection_custom_data();
    bool start_connection_axes_ratio();

    void update_st();

    const sf::Image* get_image(); // w przypadku braku zdjęcia - nullptr

    Connection_State get_connection_state();

    std::vector<Custom_Data_IO_Window::message> get_recived_message_list();

    void set_camera_view_mode(Image_Receiver::Sender_Mode mode);

    bool is_camera_view_work();
    bool is_custom_data_work();
    bool is_custom_rays_receiver();

private:
    sf::IpAddress broadcast_ip;
    sf::IpAddress remote_ip = sf::IpAddress::None;

    unsigned short port;

    std::unique_ptr<Broadcast_Connector> broadcast_connector = nullptr;
    std::unique_ptr<Image_Receiver> image_receiver = nullptr;
    std::unique_ptr<Custom_Data_IO> custom_data_io = nullptr;
    std::unique_ptr<Parameters_receiver> rays_receiver = nullptr;

    Connection_State connection_state = Connection_State::primary;

    std::vector<Custom_Data_IO_Window::message> message_list_recived;
    std::vector<Custom_Data_IO_Window::message> message_list_sended;

    Rays_intersection_analyzer* rays_intersection_analyzer_ptr = nullptr;
};


#endif //INZYNIERKA_CONNECTION_BACKEND_HPP
