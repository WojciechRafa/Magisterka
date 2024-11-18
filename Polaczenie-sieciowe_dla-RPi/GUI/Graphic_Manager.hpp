//
// Created by wpr on 19.12.22.
//

#ifndef INZYNIERKA_GRAPHIC_MANAGER_HPP
#define INZYNIERKA_GRAPHIC_MANAGER_HPP

#include <list>
#include "../Time_Object.hpp"
#include "Window.hpp"
#include "Graphic_Warehouse.hpp"
#include "Buttons_Field.hpp"
#include "Small_window.hpp"

class Graphic_Manager: public Time_Object{
public:
    Graphic_Manager(
            sf::RenderWindow& window_,
            Graphic_Warehouse& graphic_warehouse_,
            sf::Time update_period_ = sf::milliseconds(50)
            ); // w konstruktorze znajduje się międzyinnymi konfiguracja ekranu

    void update() override;

    // uwaga ! funkcja po wykonaniu zmienia actual button mesage na nothing
    Button::Button_Message get_and_delate_actual_button_mesage();

    void add_time_object_to_update(Time_Object * time_object_to_update_);
    void add_small_window_to_display(Small_window* small_window);
private:
    sf::Sprite background;

    std::vector<Buttons_Field> buttons_filed_list;

    sf::RenderWindow& window;
    Graphic_Warehouse& graphic_warehouse;

    Button::Button_Message actual_button_mesage = Button::Button_Message::nothing;

    void display();

    std::list<Time_Object*> time_objects_to_update;
    std::list<Small_window*> small_windows_ptrs;
};


#endif //INZYNIERKA_GRAPHIC_MANAGER_HPP
