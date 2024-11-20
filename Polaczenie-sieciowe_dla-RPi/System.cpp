//
// Created by wr on 12/12/22.
//

#include "System.hpp"
#include "Configs.hpp"

System::System():
    clock(Time_Object::get_clock()),
    update_period(Configs::main_loop_time),
    graphic_warehouse("../Graphic_Warehouse"),
    window(sf::VideoMode(  Configs::GUI_layout::render_window_size.x,
                                Configs::GUI_layout::render_window_size.y),
                                      Configs::GUI_layout::render_window_name),
    graphic(window,
            graphic_warehouse,
            connection_list
            ),

    image_source(Image_source::Frame_switching::automatic),

//    image_source("/home/wpr/Documents/AGH/Magisterka/Program/Video/cut_video/IMG_1264.mkv", Image_source::Frame_switching::automatic),
    raw_picture_window(Configs::GUI_layout::camera_view_window_pos),
    binarized_picture_window(Configs::GUI_layout::binarization_window_pos),
    projections_window(Configs::GUI_layout::rays_window_pos),

    projection_calculator(
                          Axes::z,
                          Axes::x,

                          Configs::computers_enum::dell,

                          projections_window.getPosition(),
                          Configs::GUI_layout::default_small_window_size,
                          Configs::GUI_layout::default_small_window_size * 0.5f,

                          false,
                          {},

                          &rays_intersection_analyzer
    ),
    binarization(Configs::is_binarization_relative)
{
    detected_objects_windows.reserve(Configs::Big_windows_parameters::displayed_big_window_list.size());
    detected_objects_displayer_list.reserve(Configs::Big_windows_parameters::displayed_big_window_list.size());
    for(auto& big_window_parameters: Configs::Big_windows_parameters::displayed_big_window_list){
        detected_objects_windows.emplace_back(big_window_parameters.window_pos, big_window_parameters.window_size);
        detected_objects_displayer_list.emplace_back(big_window_parameters.axes,
                                                     big_window_parameters.area_size,
                                                     big_window_parameters.area_zero_point,
                                                     &objects_tracker,
                                                     &detected_objects_windows.back());
    }


    image_source.set_image_and_main_time_ptr(raw_picture_with_main_time);
    raw_picture_window.set_image_ptr(&raw_picture_with_main_time->second);

    binarized_picture_window.set_image_ptr(binarized_picture.get());

    binarization.set_input_image(raw_picture_with_main_time);
    binarization.set_binarized_image(binarized_picture);
    binarization.set_parameters(bin_parameters);

    projection_calculator.set_parameters(bin_parameters);
    projection_calculator.set_additional_drawable_ptr(&projections);

    if(Configs::GUI_layout::is_projection_calculator_displayed)
        projections_window.set_additional_graphic(&projections);

    rays_intersection_analyzer.set_objects_tracker_ptr(&objects_tracker);


    graphic.add_small_window_to_display(& raw_picture_window);
    graphic.add_small_window_to_display(& binarized_picture_window);

    if(Configs::GUI_layout::is_projection_calculator_displayed)
        graphic.add_small_window_to_display(& projections_window);

    for(auto& detected_objects_window:detected_objects_windows){
        graphic.add_small_window_to_display(&detected_objects_window);
    }
}

bool System::update() {
    static sf::Time last_update_time = clock.getElapsedTime();

    sf::Time time_to_wait = update_period - (clock.getElapsedTime() - last_update_time);
    if(time_to_wait.asMicroseconds() > 0)
        sf::sleep(time_to_wait);


    last_update_time = clock.getElapsedTime();
    Time_Object::update_all_time_objets();

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

        case Button::Button_Message::turn_off_program:
            window.close();
            return true;

        case Button::Button_Message::create_new_connection_interface:
            if(connection_list.empty()) {

                auto button_field = create_button_field_to_connection_with_rays(
                        Configs::GUI_layout::first_connection_button_field.getPosition()
                        );

                // bez kamery
//                auto connection = std::make_unique<Connection>(
//                        // przyciski
//                        std::move(button_field),
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
//                        create_button_field_to_connection_with_camera_and_custom_data(),
//                        // dane
//                        message_list_displayed,
//                        message_list_sended,
////                        kamera
//                        sf::Vector2f(10, 230),
//                        sf::Vector2f(1000, 600),
//                        graphic_warehouse,
//                        50238
//                );


                auto connection = std::make_unique<Connection>(
                        // przyciski
                        std::move(button_field),

                        Configs::GUI_layout::received_rays_field.getPosition(),
                        Configs::GUI_layout::received_rays_field.getSize(),
                        sf::Vector2f(Configs::GUI_layout::received_rays_zero_point),
                        Configs::GUI_layout::received_rays_axes.first,
                        Configs::GUI_layout::received_rays_axes.second,

                        graphic_warehouse,
                        50238,
                        clock,
                        &rays_intersection_analyzer
                );


                connection_list.push_back(std::move(connection));

            // usunięcie zaznaczenia przycisku



            }
            return false;

        case Button::Button_Message::set_reference_image:
            binarization.set_reference_image();

            return false;

    }
    return false;
}

std::unique_ptr<Buttons_Field>  System::create_button_field_to_connection_with_camera_and_custom_data(sf::Vector2f button_field_pos) {
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

std::unique_ptr<Buttons_Field> System::create_button_field_to_connection_with_rays(sf::Vector2f button_field_pos) {
    sf::Vector2f button_field_size = Configs::GUI_layout::first_connection_button_field.getSize();
    sf::Color button_field_color = Configs::GUI_layout::first_connection_button_field_color;

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

    auto rays_button = Button(sf::Vector2f(210, 10),
                               buttons_field->getPosition(),
                               sf::Vector2f(80, 80),
                               Button::Button_Message::conection_establish_rays,
                               graphic_warehouse.get_texture("Rays"));

    buttons_field->add_button(std::move(cancle_button));
    buttons_field->add_button(std::move(broadcast_ip));
    buttons_field->add_button(std::move(rays_button));

    return std::move(buttons_field);
}
