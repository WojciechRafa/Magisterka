//
// Created by wr on 12/12/22.
//

#include "System.hpp"

System::System(sf::Int64 update_period_microseconds_):
    update_period_microseconds(update_period_microseconds_),
    graphic_warehouse("../Graphic_Warehouse"),
    graphic(window,
            graphic_warehouse,
            connection_list,
            50000
            )
{
}

bool System::update() {
    static sf::Clock clock;
    static sf::Int64 last_update_time_micro = clock.getElapsedTime().asMicroseconds();

        sf::Int64 time_to_wait = update_period_microseconds - (clock.getElapsedTime().asMicroseconds() - last_update_time_micro);
        if(time_to_wait > 0)
            sf::sleep(sf::microseconds(time_to_wait));

        last_update_time_micro = clock.getElapsedTime().asMicroseconds();

        // aktualizacja czasowa:

        std::list<Time_Object*> time_object_list;

        if(graphic.need_update())
            time_object_list.push_back(&graphic);

        for(auto& connection: connection_list){
            for(auto& time_object : connection->get_time_objects()){
                if(time_object->need_update())
                    time_object_list.push_back(time_object);
            }

        }

        for(auto& time_object: time_object_list){
            time_object->update();
        }


        //aktualizcaj nieczasowa (st - short time, skrót występujący w nazwach funkcji aby odróżnić je od update() który może zajmować pewien czas)
        auto connection_to_remove = connection_list.end(); // end - odpowednil null-a
        for(auto it = connection_list.begin(); it != connection_list.end(); it++){
            auto status = it->get()->update_backend_st();
            if(status == Button::Button_Message::turn_off_connection)
                connection_to_remove = it;
        }
        if(connection_to_remove != connection_list.end())
            connection_list.erase(connection_to_remove);

    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed){
            window.close();
            return true;
        }
    }

    return execute_button_message(graphic.get_and_delate_actual_button_mesage());
}


bool System::execute_button_message(Button::Button_Message message) {
    switch (message) {
        case Button::Button_Message::nothing:
            return false;
        break;

        case Button::Button_Message::turn_off_program:
            window.close();
            return true;
        break;

        case Button::Button_Message::create_new_screen:
            if(connection_list.empty()) {
                Custom_Data_IO_Window::message sended_message_1;
                Custom_Data_IO_Window::message sended_message_2;

                Custom_Data_IO_Window::message recived_message_1;
                Custom_Data_IO_Window::message recived_message_2;

                sended_message_1.is_int = true;
                sended_message_1.name   = "Wiadomosc_int";
                sended_message_1.id     = 1;

                sended_message_2.is_int = false;
                sended_message_2.name   = "Wiadomosc_float";
                sended_message_2.id     = 2;

                recived_message_1.is_int = true;
                recived_message_1.name   = "Wiadomosc_int";
                recived_message_1.id     = 1;

                recived_message_2.is_int = false;
                recived_message_2.name   = "Wiadomosc_float";
                recived_message_2.id     = 2;


                std::vector<Custom_Data_IO_Window::message> message_list_displayed = {recived_message_1, recived_message_2};
                std::vector<Custom_Data_IO_Window::message> message_list_sended = {sended_message_1, sended_message_2};



                auto connection = std::make_unique<Connection>(
                        create_button_field_to_connection(),
                        // dane
                        sf::Vector2f(1050, 230),
                        sf::Vector2f(200, 100),
                        15,
                        sf::Color::Magenta,
                        message_list_displayed,
                        message_list_sended,
//                        kamera
                        sf::Vector2f(10, 230),
                        sf::Vector2f(1000, 600),
                        graphic_warehouse,
                        50238
                );
                // bez kamery
//                auto connection = std::make_unique<Connection>(
//                        // przyciski
//                        create_button_field_to_connection(),
//                        // dane
//                        sf::Vector2f(10, 230),
//                        sf::Vector2f(200, 100),
//                        15,
//                        sf::Color::Magenta,
//                        message_list_displayed,
//                        message_list_sended,
//
//                        graphic_warehouse,
//                        50238
//                );

//                // bez danych liczbowych
//                auto connection = std::make_unique<Connection>(
//                        // przyciski
//                        create_button_field_to_connection(),
//                        // dane
//                        message_list_displayed,
//                        message_list_sended,
////                        kamera
//                        sf::Vector2f(10, 230),
//                        sf::Vector2f(1000, 600),
//                        graphic_warehouse,
//                        50238
//                );

                connection_list.push_back(std::move(connection));

            // usunięcie zaznaczenia przycisku



            }
            return false;


    }
    return false;
}

std::unique_ptr<Buttons_Field>  System::create_button_field_to_connection() {
    sf::Vector2f button_field_pos = sf::Vector2f(10, 120);
    sf::Vector2f button_field_size = sf::Vector2f(1100, 100);
    sf::Color button_field_color = sf::Color::Cyan;

    auto  buttons_field = std::make_unique<Buttons_Field>(
                                    button_field_pos,
                                            button_field_size,
                                            button_field_color
                                                 );

    auto cancle_button = Button(sf::Vector2f(10, 10),
                                buttons_field->getPosition(),
                                sf::Vector2f(80, 80),
                                Button::Button_Message::turn_off_connection,
                                graphic_warehouse.get_texture("Cancel"));

    auto broadcast_ip = Button(sf::Vector2f(110, 10),
                               buttons_field->getPosition(),
                               sf::Vector2f(80, 80),
                               Button::Button_Message::broadcast_ip_process,
                               graphic_warehouse.get_texture("Broadcast"));

    auto connect_camera_view = Button(sf::Vector2f(220, 10),
                                      buttons_field->getPosition(),
                                      sf::Vector2f(80, 80),
                                      Button::Button_Message::conection_establish_camera_view,
                                      graphic_warehouse.get_texture("Connection_Camera_View"));

    auto connect_custom_data = Button(sf::Vector2f(330, 10),
                                      buttons_field->getPosition(),
                                      sf::Vector2f(80, 80),
                                      Button::Button_Message::conection_establish_custom_data,
                                      graphic_warehouse.get_texture("Connection_Data_Exchange"));

    auto set_camera_mode_const_20_fps = Button(sf::Vector2f(440, 10),
                                               buttons_field->getPosition(),
                                               sf::Vector2f(80, 80),
                                               Button::Button_Message::set_camera_mode_const_20_fps,
                                               graphic_warehouse.get_texture("20_fps"));

    auto set_camera_mode_x1 = Button(sf::Vector2f(550, 10),
                                     buttons_field->getPosition(),
                                     sf::Vector2f(80, 80),
                                     Button::Button_Message::set_camera_mode_x1,
                                     graphic_warehouse.get_texture("x1"));


    auto set_camera_mode_x0_5 = Button(sf::Vector2f(660, 10),
                                       buttons_field->getPosition(),
                                       sf::Vector2f(80, 80),
                                       Button::Button_Message::set_camera_mode_x0_5,
                                       graphic_warehouse.get_texture("x0_5"));


    buttons_field->add_button(std::move(cancle_button));
    buttons_field->add_button(std::move(broadcast_ip));
    buttons_field->add_button(std::move(connect_camera_view));
    buttons_field->add_button(std::move(connect_custom_data));

    buttons_field->add_button(std::move(set_camera_mode_const_20_fps));
    buttons_field->add_button(std::move(set_camera_mode_x1));
    buttons_field->add_button(std::move(set_camera_mode_x0_5));

    return std::move(buttons_field);
}
