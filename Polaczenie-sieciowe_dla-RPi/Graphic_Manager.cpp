//
// Created by wpr on 19.12.22.
//

#include "Graphic_Manager.hpp"
#include "main_functions.hpp"

Graphic_Manager::Graphic_Manager(Window &window_,
                                 Graphic_Warehouse &graphic_warehouse_,
                                 std::vector<std::unique_ptr<Connection>>& connection_list_,
                                 sf::Int64 update_period_microseconds_
                                 ):
Time_Object(update_period_microseconds_),
window(window_),
graphic_warehouse(graphic_warehouse_),
connection_list(connection_list_)
{
    background.setTexture(graphic_warehouse.get_texture("Background"));
    resize_sprite(window.getSize(), background);

    auto main_menu = Buttons_Field(sf::Vector2f(10, 10),
                                   sf::Vector2f(1100, 100),
                                   sf::Color::White
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
                                 Button::Button_Message::create_new_screen,
                                 graphic_warehouse.get_texture("Plus")

    );

    main_menu.add_button(std::move(cancle_button));
    main_menu.add_button(std::move(new_connection));

    buttons_filed_list.push_back(std::move(main_menu));
}

void Graphic_Manager::update() {
    auto mouse_relative_to_windows = sf::Mouse::getPosition(window);
    auto world_relative_to_world = static_cast<sf::Vector2i>(window.mapPixelToCoords(mouse_relative_to_windows));

    for(auto& button_field: buttons_filed_list){
        auto button_field_status = button_field.update_field(world_relative_to_world);

        if (button_field_status != Button::Button_Message::nothing)
            actual_button_mesage = button_field_status;

        if(actual_button_mesage == Button::Button_Message::create_new_screen) {
            button_field.set_button_mode(Button::Button_Message::create_new_screen, false); // dla tego przycisku, wyświetlenie nie jest zapamiętywane
        }
    }

    for(auto& connection: connection_list){
        auto button_field_status =  connection->update_frontend_st(world_relative_to_world);

        if (button_field_status != Button::Button_Message::nothing) {
            actual_button_mesage = button_field_status; // uwaga może dość do nadpisania
        }
        connection->update_image();
    }

    display();

    last_update_time = clock.getElapsedTime().asMicroseconds();
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
    }

    window.display();
}

Button::Button_Message Graphic_Manager::get_and_delate_actual_button_mesage() {
    auto message_copy = actual_button_mesage;
    actual_button_mesage = Button::Button_Message::nothing;
    return message_copy;
}

