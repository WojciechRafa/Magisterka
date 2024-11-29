#pragma once

#include <SFML/Network.hpp>
#include "Pernament_Connector.hpp"
#include "../Image_Analysis/Camera.hpp"
#include "Sended_struct.hpp"




class Image_Sender: public Permanent_Connector{
public:
    enum class Sender_Mode{
        const_20_fps,
        scale_x1,
        scale_x0_5
    };

	Image_Sender(unsigned short port_, sf::IpAddress remote_dev_ip_);

    void update() override;

    bool need_update() override;
private:
    // sends the next frame and takes a picture of the next one, if it was not able to do it returns false
    bool try_send_new_image();
    bool update_image_and_number();

    void update_establish_connection_mode();
    void update_pernament_communication_mode();


    const int max_frame_delay = 5; // frame delay - różnica między najwyższym numerem wysłanego zdjęcia i odebranym numerem zwrotnym
    sf::Int32 last_recived_frame_number = 0;
    sf::Int32 last_sended_frame_number = 0;

    Camera camera;
    Image_and_number image_and_number;

    bool need_update_recive();
    bool need_update_send();

    sf::Int64 update_period_microseconds_send   = 40000;
    sf::Int64 update_period_microseconds_recive = 20000;
    sf::Int64 last_update_time_send = 0;
    sf::Int64 last_update_time_recive = 0;

    Sender_Mode sender_mode = Sender_Mode::scale_x1;

    // time mangage
    const int number_of_remembered_samples = 20;

    const float max_compress_relative_to_whole_time = 0.6;
    const float min_compress_relative_to_whole_time = 0.4;
    const float change_threshold_compress = 1.5;

    // Since changing the file scale is associated with a change in quality, different types of thresholds have
    // been adopted for it, a time that is low enough to increase the quality and long enough to cause its deterioration
    const sf::Int64 quality_improvment_threshold = 50000;
    const sf::Int64 quality_deterioration_threshold = quality_improvment_threshold * 2;

    // dla trybu prostego
    sf::Int64 last_average_time_compressed = -1; // -1 -> wartość domyślna
    sf::Int64 last_average_time_not_compressed = -1;

    void change_sender_mode(Sender_Mode mode);
    // the advanced method is one in which the target speed is set and it is possible to smoothly change the scale
    bool is_complex_image_management_method_used();
    void manage_sender_mode();

    // functions that return true if any changes have been made
    bool manage_time_simple(sf::Int64 average_cycle_time);
    bool manage_time_complex(sf::Int64 average_cycle_time);

    std::list<sf::Int64> last_img_send_time = {};
    sf::Int64 compress_time;

};
