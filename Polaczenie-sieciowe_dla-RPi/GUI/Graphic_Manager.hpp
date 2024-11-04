//
// Created by wpr on 19.12.22.
//

#ifndef INZYNIERKA_GRAPHIC_MANAGER_HPP
#define INZYNIERKA_GRAPHIC_MANAGER_HPP

#include "../Time_Object.hpp"
#include "Window.hpp"
#include "Graphic_Warehouse.hpp"
#include "Buttons_Field.hpp"
#include "../Connection/Connection.hpp"
#include "Small_window.hpp"

class Graphic_Manager: public Time_Object{
public:
    Graphic_Manager(
            Window& window_,
            Graphic_Warehouse& graphic_warehouse_,
            std::vector<std::unique_ptr<Connection>>& connection_list_
    );// w konstruktorze znajduje się międzyinnymi konfiguracja ekranu

    void update() override;

    // uwaga ! funkcja po wykonaniu zmienia actual button mesage na nothing
    Button::Button_Message get_and_delate_actual_button_mesage();

    void add_time_object_to_update(Time_Object * time_object_to_update_);
    void add_small_window_to_display(Small_window* small_window);

    std::string get_name() override{return "Graphic Manager";};
private:
    sf::Sprite background;

    std::vector<Buttons_Field> buttons_filed_list;

    Window& window;
    Graphic_Warehouse& graphic_warehouse;

    // lista powinna być wykorzystywana tylko do aktualizacji grafiki
    const std::vector<std::unique_ptr<Connection>>& connection_list;

    Button::Button_Message actual_button_mesage = Button::Button_Message::nothing;

    void display();

    std::list<Time_Object*> time_objects_to_update;
    std::list<Small_window*> small_windows;
};


#endif //INZYNIERKA_GRAPHIC_MANAGER_HPP
