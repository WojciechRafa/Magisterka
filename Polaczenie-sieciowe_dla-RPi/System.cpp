//
// Created by wr on 12/27/22.
//

#include "System.hpp"

System::System():
    update_period(Configs::main_loop_time),
    graphic_warehouse("../Graphic_Warehouse"),
    graphic(window,
        graphic_warehouse),
    image_source(Image_source::Frame_switching::automatic),

//    image_source("/home/wpr/Documents/AGH/Magisterka/Program/Video/cut_video/IMG_1264.mkv", Image_source::Frame_switching::automatic),
    raw_picture_window(standard_window_size, sf::Vector2f(10, 120)),
    binarized_picture_window(standard_window_size, sf::Vector2f(320, 120)),
    projections_window(standard_window_size, sf::Vector2f(630, 120), sf::Color::White),

    projection_calculator(
                          Projection_image_calculate::axes::z,
                          Projection_image_calculate::axes::x,
                          projections_window.getPosition(),
                          standard_window_size,
                          standard_window_size * 0.5f
                          ),
    binarization(Configs::is_binarization_relative)
    {
    broadcast_connector = std::make_unique<Broadcast_Connector>(port);

    image_source.set_image_and_main_time_ptr(raw_picture_with_main_time);
    raw_picture_window.set_image_ptr(&raw_picture_with_main_time->second);

    binarized_picture_window.set_image_ptr(binarized_picture.get());

    binarization.set_input_image(raw_picture_with_main_time);
    binarization.set_binarized_image(binarized_picture);

    binarization.set_parameters(bin_parameters);

    projection_calculator.set_parameters(bin_parameters);
    projection_calculator.set_additional_graphic(projections);
    projection_calculator.set_sent_parameters_ptr(&sent_parameters);

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
    static sf::Time last_update_time = clock.getElapsedTime();

    sf::Time time_to_wait = update_period - (clock.getElapsedTime() - last_update_time);
    if(time_to_wait.asMicroseconds() > 0)
        sf::sleep(time_to_wait);


    last_update_time = clock.getElapsedTime();
    Time_Object::update_all_time_objets();


        // ST - short time
    if(connection_state == Connection_State::broadcast_listen and
       broadcast_connector != nullptr and
       broadcast_connector->get_mode() == Broadcast_Connector::b_connector_mode::connection_confirmed){
        remote_ip_address = broadcast_connector->get_remote_ip();

        connection_state = Connection_State::both_wait_to_pernamant_connect;

        parameter_sender = std::make_unique<Parameter_sender>(port, remote_ip_address);
        // parameter_sender->set_update_period(1000);
//        parameter_sender->set_vectors_list_ptr(&vectors_list);
        parameter_sender->set_sent_parameters_ptr(&sent_parameters);

        std::cout<<"Procedura zawiązywania polaczenia przez Broadcast zakonczona !"<<std::endl;
        broadcast_connector = nullptr;
    }

    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::EventType::KeyPressed){
            switch (event.key.code) {
                case sf::Keyboard::R:
                    binarization.set_reference_image();
                    break;
                default:
                    break;
            }
        }

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

        case Button::Button_Message::set_reference_image:
            binarization.set_reference_image();

            return false;

    }
    return false;
}
