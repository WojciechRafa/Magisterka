//
// Created by wr on 11/24/22.
//

#ifndef INZYNIERKA_IMAGE_RECEIVER_HPP
#define INZYNIERKA_IMAGE_RECEIVER_HPP

#include "Pernament_Connector.hpp"
#include "SFML/Graphics.hpp"

class Image_Receiver: public Pernament_Connector{
public:
    enum class Sender_Mode{
        const_20_fps,
        scale_x1,
        scale_x0_5
    };

    Image_Receiver() = default;
    Image_Receiver(unsigned short port_, sf::IpAddress remote_dev_ip_);

    void update() override;

    sf::Image* get_image();

    void set_camera_view_mode(Sender_Mode mode);
private:
    void send_img_nr_and_transmission_parameters();
    bool receive_img(); // last img_nr jest zmieniane tylko przy odebraniu zdjęcia

    sf::Int32 last_img_nr = 0;
    sf::Image image;
    bool is_image_valid = false;

    Sender_Mode sender_mode = Sender_Mode::scale_x1;
};


#endif //INZYNIERKA_IMAGE_RECEIVER_HPP
