//
// Created by wr on 12/27/22.
//

#include "System.hpp"

System::System(sf::Int64 update_period_microseconds_):
    update_period_microseconds(update_period_microseconds_),
    graphic_warehouse("../Graphic_Warehouse"),
    graphic(window,
        graphic_warehouse,
    50000),
    image_source(Image_source::Frame_switching::automatic),

//    image_source("/home/wpr/Documents/AGH/Magisterka/Program/Video/cut_video/IMG_1264.mkv", Image_source::Frame_switching::automatic),
    raw_picture_window(standard_window_size, sf::Vector2f(10, 120)),
    binarized_picture_window(standard_window_size, sf::Vector2f(320, 120)),
    projections_window(standard_window_size, sf::Vector2f(630, 120), sf::Color::White),

    projection_calculator(Projection_image_calculate::axes::z,
                          Projection_image_calculate::axes::x,
                          projections_window.getPosition(),
                          standard_window_size,
                          standard_window_size * 0.5f
                          )
    {
    broadcast_connector = std::make_unique<Broadcast_Connector>(port);

    image_source.set_image_ptr(raw_picture);
    raw_picture_window.set_image_ptr(raw_picture);

    binarized_picture_window.set_image_ptr(binarized_picture);

    binarization.set_input_image(raw_picture);
    binarization.set_binarized_image(binarized_picture);
//    binarization.set_centroids(centroids);
//    binarization.set_stats(stats);
    binarization.set_parameters(bin_parameters);

//    projection_calculator.set_stats(stats);
//    projection_calculator.set_centroids(centroids);
    projection_calculator.set_parameters(bin_parameters);
    projection_calculator.set_additional_graphic(projections);

    projections_window.set_additional_graphic(projections);
    projection_calculator.set_vectors_list(&vectors_list);

    graphic.add_time_object_to_update(& raw_picture_window);
    graphic.add_small_window_to_display(& raw_picture_window);
    graphic.add_time_object_to_update(& binarized_picture_window);
    graphic.add_small_window_to_display(& binarized_picture_window);
    graphic.add_time_object_to_update(&projections_window);
    graphic.add_small_window_to_display(&projections_window);
    }

bool System::update() {

    static sf::Clock clock;
    static sf::Int64 last_update_time_micro = clock.getElapsedTime().asMicroseconds();

    sf::Int64 time_to_wait = update_period_microseconds - (clock.getElapsedTime().asMicroseconds() - last_update_time_micro);
    if(time_to_wait > 0)
        sf::sleep(sf::microseconds(time_to_wait));


    last_update_time_micro = clock.getElapsedTime().asMicroseconds();


    std::list<Time_Object*> time_object_list;

    if(connection_state == Connection_State::broadcast_listen and broadcast_connector != nullptr) {
        if (broadcast_connector->need_update()) {
            time_object_list.push_back(broadcast_connector.get());
        }
    }

    if(connection_state != Connection_State::broadcast_listen){
        time_object_list.push_back(ray_sender.get());

    }

    // get_new_frame
    time_object_list.push_back(&image_source);
    time_object_list.push_back(&binarization);
    time_object_list.push_back(&projection_calculator);

    // graphic:
    if(graphic.need_update())
        time_object_list.push_back(&graphic);

    for(auto& time_object: time_object_list){
        time_object->update();
    }
    time_object_list.clear();


        // ST - short time
    if(connection_state == Connection_State::broadcast_listen and
       broadcast_connector != nullptr and
       broadcast_connector->get_mode() == Broadcast_Connector::b_connector_mode::connection_confirmed){
        remote_ip_address = broadcast_connector->get_remote_ip();

        connection_state = Connection_State::both_wait_to_pernamant_connect;

        ray_sender = std::make_unique<Rays_sender>(port, remote_ip_address, clock);
        ray_sender->set_vectors_list_ptr(&vectors_list);

        std::cout<<"Procedura zawiązywania polaczenia przez Broadcast zakonczona !"<<std::endl;
        broadcast_connector = nullptr;
    }

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

//                auto connection = std::make_unique<Connection>(
//                        create_button_field_to_connection(),
//                        // dane
//                        sf::Vector2f(1050, 230),
//                        sf::Vector2f(200, 100),
//                        15,
//                        sf::Color::Magenta,
//                        message_list_displayed,
//                        message_list_sended,
////                        kamera
//                        sf::Vector2f(10, 230),
//                        sf::Vector2f(1000, 600),
//                        graphic_warehouse,
//                        50238
//                );
//                // bez kamery
////                auto connection = std::make_unique<Connection>(
////                        // przyciski
////                        create_button_field_to_connection(),
////                        // dane
////                        sf::Vector2f(10, 230),
////                        sf::Vector2f(200, 100),
////                        15,
////                        sf::Color::Magenta,
////                        message_list_displayed,
////                        message_list_sended,
////
////                        graphic_warehouse,
////                        50238
////                );
//
////                // bez danych liczbowych
////                auto connection = std::make_unique<Connection>(
////                        // przyciski
////                        create_button_field_to_connection(),
////                        // dane
////                        message_list_displayed,
////                        message_list_sended,
//////                        kamera
////                        sf::Vector2f(10, 230),
////                        sf::Vector2f(1000, 600),
////                        graphic_warehouse,
////                        50238
////                );
//
//                connection_list.push_back(std::move(connection));
//
//                // usunięcie zaznaczenia przycisku
//
//
//
//            }
            return false;


    }
    return false;
}
