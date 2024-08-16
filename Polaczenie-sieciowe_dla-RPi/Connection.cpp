//
// Created by wr on 12/17/22.
//

#include "Connection.hpp"

Connection::Connection(
                    std::unique_ptr<Buttons_Field> button_filed_,

                       sf::Vector2f custom_data_view_pos,
                       sf::Vector2f custom_data_view_size,
                       unsigned int character_size,
                       sf::Color custom_data_background_color,
                       std::vector<Custom_Data_IO_Window::message>& message_list_displayed,
                       std::vector<Custom_Data_IO_Window::message>& message_list_sended,

                       sf::Vector2f camera_view_pos,
                       sf::Vector2f camera_view_size,
                       Graphic_Warehouse& graphic_warehouse,
                       unsigned short port
                       ):
        frontend(
                std::move(button_filed_),
                    custom_data_view_pos,
                    custom_data_view_size,
                    character_size,
                    custom_data_background_color,
                    message_list_displayed,

                    camera_view_pos,
                    camera_view_size,
                    graphic_warehouse
                    ),
        backend(port,message_list_sended ,message_list_displayed)
                    {}

Connection::Connection(std::unique_ptr<Buttons_Field> button_filed_, sf::Vector2f custom_data_view_pos,
                       sf::Vector2f custom_data_view_size, unsigned int character_size,
                       sf::Color custom_data_background_color,
                       std::vector<Custom_Data_IO_Window::message> &message_list_displayed,
                       std::vector<Custom_Data_IO_Window::message> &message_list_sended,
                       Graphic_Warehouse &graphic_warehouse,
                       unsigned short port) :
        frontend(
                std::move(button_filed_),
                custom_data_view_pos,
                custom_data_view_size,
                character_size,
                custom_data_background_color,
                message_list_displayed,

                graphic_warehouse
        ),
        backend(port,message_list_sended ,message_list_displayed)
{}

Connection::Connection(std::unique_ptr<Buttons_Field> button_filed_,
                       std::vector<Custom_Data_IO_Window::message> &message_list_displayed,
                       std::vector<Custom_Data_IO_Window::message> &message_list_sended, sf::Vector2f camera_view_pos,
                       sf::Vector2f camera_view_size,
                       Graphic_Warehouse &graphic_warehouse,
                       unsigned short port) :
        frontend(
                std::move(button_filed_),

                camera_view_pos,
                camera_view_size,
                graphic_warehouse
        ),
        backend(port,message_list_sended ,message_list_displayed)
{}




std::vector<sf::Drawable *> Connection::get_figures_list() {
    return frontend.get_figures_list();
}

Button::Button_Message Connection::update_frontend_st(sf::Vector2i mouse_pos_relative_to_window) {
    //wyświetlanie danych na ekranie
    for(auto& message : backend.get_recived_message_list()) {
        if (message.is_int)
            frontend.custom_data_update_variable_by_name_int(message.name, message.data_int);
        else
            frontend.custom_data_update_variable_by_name_float(message.name, message.data_float);

    }
    // usunięcie zaznaczenia aktywności połączenia po zakończeniu trybu broadcast
    static bool was_backend_mode_last_time_broadcast = false;
    if(was_backend_mode_last_time_broadcast and backend.get_connection_state() != Connection_Backend::Connection_State::broadcast){
        frontend.set_button_mode(Button::Button_Message::broadcast_ip_process, false);
    }
    was_backend_mode_last_time_broadcast = backend.get_connection_state() == Connection_Backend::Connection_State::broadcast;


    return frontend.update_st(mouse_pos_relative_to_window);
}

Button::Button_Message Connection::update_backend_st() {
    auto message = frontend.get_and_delate_actual_button_mesage();

    if(message == Button::Button_Message::broadcast_ip_process){
       backend.start_looking_for_another_device();

    }else if(message == Button::Button_Message::conection_establish_camera_view){
        backend.start_connection_camera_view();
        frontend.set_button_mode(Button::Button_Message::broadcast_ip_process, false);
        frontend.set_button_mode(Button::Button_Message::set_camera_mode_x1, true); // tryb domyślny

    } else if(message == Button::Button_Message::conection_establish_custom_data){
        backend.start_connection_custom_data();
        frontend.set_button_mode(Button::Button_Message::broadcast_ip_process, false);
    }
    if(backend.is_camera_view_work()){
        if(message == Button::Button_Message::set_camera_mode_const_20_fps){
            backend.set_camera_view_mode(Image_Receiver::Sender_Mode::const_20_fps);
            frontend.set_button_mode(Button::Button_Message::set_camera_mode_x1, false);
            frontend.set_button_mode(Button::Button_Message::set_camera_mode_x0_5, false);

        } else if(message == Button::Button_Message::set_camera_mode_x1){
                backend.set_camera_view_mode(Image_Receiver::Sender_Mode::scale_x1);
                frontend.set_button_mode(Button::Button_Message::set_camera_mode_const_20_fps, false);
                frontend.set_button_mode(Button::Button_Message::set_camera_mode_x0_5, false);

        } else if(message == Button::Button_Message::set_camera_mode_x0_5){
            backend.set_camera_view_mode(Image_Receiver::Sender_Mode::scale_x0_5);
            frontend.set_button_mode(Button::Button_Message::set_camera_mode_const_20_fps, false);
            frontend.set_button_mode(Button::Button_Message::set_camera_mode_x1, false);

        }
    }

    backend.update_st();

    return message;
}


void Connection::update_image() {
    if (backend.get_image() != nullptr) {
        frontend.update_image(*backend.get_image());
    }
}
std::vector<Time_Object *> Connection::get_time_objects() {
    return backend.get_time_object_list();
}


