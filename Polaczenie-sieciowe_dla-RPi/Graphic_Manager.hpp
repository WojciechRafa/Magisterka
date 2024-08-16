//
// Created by wpr on 19.12.22.
//

#ifndef INZYNIERKA_GRAPHIC_MANAGER_HPP
#define INZYNIERKA_GRAPHIC_MANAGER_HPP

#include "Time_Object.hpp"
#include "Window.hpp"
#include "Graphic_Warehouse.hpp"
#include "Buttons_Field.hpp"
#include "Connection.hpp"

class Graphic_Manager: public Time_Object{
public:
    Graphic_Manager(
            Window& window_,
            Graphic_Warehouse& graphic_warehouse_,
            std::vector<std::unique_ptr<Connection>>& connection_list_,
            sf::Int64 update_period_microseconds_
            ); // w konstruktorze znajduje się międzyinnymi konfiguracja ekranu

    void update() override;

    // uwaga ! funkcja po wykonaniu zmienia actual button mesage na nothing
    Button::Button_Message get_and_delate_actual_button_mesage();
private:
    sf::Sprite background;

    std::vector<Buttons_Field> buttons_filed_list;

    Window& window;
    Graphic_Warehouse& graphic_warehouse;

    // lista powinna być wykorzystywana tylko do aktualizacji grafiki
    const std::vector<std::unique_ptr<Connection>>& connection_list;

    Button::Button_Message actual_button_mesage = Button::Button_Message::nothing;

    void display();
};


#endif //INZYNIERKA_GRAPHIC_MANAGER_HPP
