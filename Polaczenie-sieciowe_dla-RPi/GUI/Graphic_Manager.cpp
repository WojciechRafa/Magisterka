//
// Created by wpr on 19.12.22.
//

#include "Graphic_Manager.hpp"
#include "../main_functions.hpp"

Graphic_Manager::Graphic_Manager(sf::RenderWindow &window_,
                                 Graphic_Warehouse &graphic_warehouse_,
                                 std::vector<std::unique_ptr<Connection>>& connection_list_
                                 ):
window(window_),
graphic_warehouse(graphic_warehouse_),
connection_list(connection_list_)
{
    background.setTexture(graphic_warehouse.get_texture("Background"));
    resize_sprite(window.getSize(), background);

    auto main_menu = Buttons_Field(Configs::GUI_layout::main_button_field.getPosition(),
                                   Configs::GUI_layout::main_button_field.getSize(),
                                   Configs::GUI_layout::main_button_field_color
    );


    auto cancle_button = Button(sf::Vector2f(10, 10),
                                main_menu.getPosition(),
                                sf::Vector2f(80, 80),
                                Button::Button_Message::turn_off_program,
                                graphic_warehouse.get_texture("Cancel")
    );

    auto new_connection = Button(sf::Vector2f(110, 10),
                                 main_menu.getPosition(),
                                 sf::Vector2f(80, 80),
                                 Button::Button_Message::create_new_connection_interface,
                                 graphic_warehouse.get_texture("Plus")
    );


    auto reference_image_button = Button(sf::Vector2f(210, 10),
                                         main_menu.getPosition(),
                                         sf::Vector2f(80, 80),
                                         Button::Button_Message::set_reference_image,
                                         graphic_warehouse.get_texture("Set_reference_image")
                                         );

    main_menu.add_button(std::move(cancle_button));
    main_menu.add_button(std::move(new_connection));

    if(not Configs::is_binarization_relative){
        main_menu.add_button(std::move(reference_image_button));
    }
    buttons_filed_list.push_back(std::move(main_menu));
}

void Graphic_Manager::update() {
    auto mouse_relative_to_windows = sf::Mouse::getPosition(window);
    auto world_relative_to_world = static_cast<sf::Vector2i>(window.mapPixelToCoords(mouse_relative_to_windows));

    for(auto& button_field: buttons_filed_list){
        auto button_field_status = button_field.update_field(world_relative_to_world);

        if (button_field_status != Button::Button_Message::nothing)
            actual_button_mesage = button_field_status;

        if(actual_button_mesage == Button::Button_Message::create_new_connection_interface) {
            button_field.set_button_mode(Button::Button_Message::create_new_connection_interface, false);
        }
    }

    for(auto& connection: connection_list){
        auto button_field_status =  connection->update_frontend_st(world_relative_to_world);

        if (button_field_status != Button::Button_Message::nothing) {
            actual_button_mesage = button_field_status; // can be overwritten
        }
        connection->update_image();
    }

    display();

    last_update_time = clock.getElapsedTime();
}

void Graphic_Manager::display() {
    window.draw(background);

    for(auto& button_field: buttons_filed_list){
        window.draw(button_field);
        for(auto button: button_field.get_figures_list()){
            window.draw(*button);
        }
    }
    for(auto& connection: connection_list){
        for(auto& shape: connection->get_figures_list()){
            window.draw(*shape);
        }

        for(auto& shape_list: *connection->get_additional_graphic_lists()){
            for(auto & shape: shape_list) {
                window.draw(*shape);
            }
        }
    }

    for(auto small_window: small_windows_ptrs){
        if(small_window != nullptr) {
            window.draw(* small_window);

            if(small_window->get_additional_graphic() != nullptr){
                for(auto& graphic: *small_window->get_additional_graphic()){
                    window.draw(*graphic);
                }
            }
        }
    }

    window.display();
}

Button::Button_Message Graphic_Manager::get_and_delate_actual_button_mesage() {
    auto message_copy = actual_button_mesage;
    actual_button_mesage = Button::Button_Message::nothing;



    return message_copy;
}

void Graphic_Manager::add_small_window_to_display(Small_window* small_window) {
    small_windows_ptrs.push_back(small_window);
}
