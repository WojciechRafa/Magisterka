//
// Created by wr on 12/12/22.
//

#ifndef INZYNIERKA_SYSTEM_HPP
#define INZYNIERKA_SYSTEM_HPP

#include "Window.hpp"
#include "Buttons_Field.hpp"
#include "Graphic_Warehouse.hpp"
#include "main_functions.hpp"
#include "Connection.hpp"
#include "Graphic_Manager.hpp"

class System {
public:
    explicit System(sf::Int64 update_period_microseconds_ = 1000); // domyślne maksymalne tempo aktualizacji 1ms

    bool update();
private:
    bool execute_button_message(Button::Button_Message message); // zwraca informacje o tym czy należy zamknąć program

    Window window;

    std::vector<std::unique_ptr<Connection>> connection_list;

    sf::Event event{};
    Graphic_Warehouse graphic_warehouse;

    Graphic_Manager graphic;

    sf::Int64 update_period_microseconds;

    std::unique_ptr<Buttons_Field>  create_button_field_to_connection();
};


#endif //INZYNIERKA_SYSTEM_HPP
