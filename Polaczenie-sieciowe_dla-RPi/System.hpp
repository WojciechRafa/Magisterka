//
// Created by wr on 12/27/22.
//

#ifndef INZYNIERKA_SYSTEM_HPP
#define INZYNIERKA_SYSTEM_HPP

#include "Network/Broadcast_connector.hpp"

// graphic:
#include "GUI/Buttons_Field.hpp"
#include "GUI/Graphic_Warehouse.hpp"
#include "GUI/Graphic_Manager.hpp"

#include "GUI/Small_window.hpp"
#include "Image_source.hpp"
#include "Image_Analysis/Binarization.hpp"
#include "Image_Analysis/Projection_image_calculator.hpp"
#include "Network/Parameter_sender.hpp"

class System {
public:
    explicit System();

    bool update();

    enum class Connection_State{
        broadcast_listen,
        bind_error,
        both_wait_to_pernamant_connect,
        image_reciver_work,
        custom_data_io_work,
        both_work
    };
private:
    sf::Time update_period;
    Connection_State connection_state = Connection_State::broadcast_listen;
    unsigned short port = 50238;
    sf::IpAddress remote_ip_address = sf::IpAddress::None;

    std::unique_ptr<Broadcast_Connector> broadcast_connector = nullptr;
    // std::unique_ptr<Image_Sender> image_sender = nullptr;
    // std::unique_ptr<Custom_Data_IO> custom_data_io = nullptr;
    std::unique_ptr<Parameter_sender> parameter_sender = nullptr;

//    void configure_custom_data();
    // graphic:
    sf::RenderWindow window;
    sf::Event event{};
    Graphic_Warehouse graphic_warehouse;

    Graphic_Manager graphic;
    bool execute_button_message(Button::Button_Message message);

    const sf::Vector2f standard_window_size = sf::Vector2f(300, 200);

    Image_source image_source;
    Binarization binarization;

    std::shared_ptr<std::pair<sf::Time, cv::Mat>> raw_picture_with_main_time
        = std::make_shared<std::pair<sf::Time, cv::Mat>>();
    std::shared_ptr<cv::Mat> binarized_picture = std::make_shared<cv::Mat>();

    Small_window raw_picture_window;
    Small_window binarized_picture_window;
    Small_window projections_window;

    std::shared_ptr<Binarization::Binarized_parameters> bin_parameters =
            std::make_shared<Binarization::Binarized_parameters>();

    std::vector<std::unique_ptr<sf::Drawable>> projections = std::vector<std::unique_ptr<sf::Drawable>>();
    Sent_parameters sent_parameters = {};

    Projection_image_calculator projection_calculator;

};


#endif //INZYNIERKA_SYSTEM_HPP
