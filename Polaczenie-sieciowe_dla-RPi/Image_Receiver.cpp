//
// Created by wr on 11/24/22.
//

#include "Image_Receiver.hpp"
#include "sended_struct.hpp"
#include "main_functions.hpp"

Image_Receiver::Image_Receiver(unsigned short port_, sf::IpAddress remote_dev_ip_):
Pernament_Connector(port_, remote_dev_ip_) {
    update_period_microseconds = 500000; // pierwotny czas aktualziacji wykorzystywany przy nawiązywaniu połączenia
}


void Image_Receiver::send_img_nr_and_transmission_parameters() {
    sf::Packet packet;
    packet << last_img_nr << static_cast<sf::Int8>(sender_mode);

    auto status = send(packet);
}


bool Image_Receiver::receive_img() {
    sf::Packet packet;

    setBlocking(false);
    auto status = receive(packet);
    setBlocking(true);

    if(status == sf::Socket::Status::Done) {
        Image_and_number image_and_number;
        packet >> image_and_number;

        auto raw_data = image_and_number.img_vec;

        cv::Mat img_mat;
        if(image_and_number.is_compressed) {
            std::vector<int> compression_params = {cv::IMWRITE_JPEG_QUALITY, 95};
            img_mat = cv::imdecode(cv::InputArray(raw_data), cv::IMREAD_COLOR);
        }else{
            img_mat = cv::Mat(image_and_number.rows, image_and_number.cols, image_and_number.type, raw_data.data());
        }

        last_img_nr = static_cast<sf::Int32> (image_and_number.image_number);

        cv_Mat_2_sf_Image(img_mat, image);

        return true;
    }
    return false;
}

void Image_Receiver::update() {
    if(mode == Pernament_Connector::p_connector_mode::establish_connection){
        Pernament_Connector::update();
        if(get_mode() == Pernament_Connector::p_connector_mode::pernament_communication){
            update_period_microseconds = 50000;
        }
        // czas jest aktualziowany w Pernament_Connector::update();
    }else{
        bool is_image_received = receive_img();
        if(is_image_received){
            send_img_nr_and_transmission_parameters();
        }
        last_update_time = clock.getElapsedTime().asMicroseconds();
    }
}

sf::Image* Image_Receiver::get_image() {
    return &image;
}

void Image_Receiver::set_camera_view_mode(Image_Receiver::Sender_Mode mode) {
    sender_mode = mode;
}

