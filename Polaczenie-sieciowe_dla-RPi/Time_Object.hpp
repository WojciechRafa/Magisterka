//
// Created by wr on 12/17/22.
//

#ifndef INZYNIERKA_TIME_OBJECT_HPP
#define INZYNIERKA_TIME_OBJECT_HPP

#include <SFML/System/Clock.hpp>
#include <list>
#include <memory>
#include "Configs.hpp"
// klasa będzie wykorzystywana do obsługi wszystkich obiektów które muszą być aktualizowane w regularnych okresach czas

class Time_Object {
public:
    Time_Object();
    ~Time_Object();
    explicit Time_Object(sf::Time update_period_);

    virtual void update() = 0; // Aktualizować last_update_time
    static std::list<Time_Object*>* get_all_time_objects_pointers();
    static void update_all_time_objets();

    virtual bool need_update(); // wiele instancji może nadpisywać tą funkcję

    static sf::Clock& get_clock();
protected:
    // WAŻNE ! Aktualizować last update time !
    sf::Time update_period;

    static sf::Clock clock;
    sf::Time last_update_time = sf::milliseconds(0);
private:
    static std::list<Time_Object*> all_time_objects_pointers;

    const sf::Time default_update_period_microseconds = Configs::default_update_time;
};

#endif //INZYNIERKA_TIME_OBJECT_HPP
