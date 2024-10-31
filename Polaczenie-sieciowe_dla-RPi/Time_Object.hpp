//
// Created by wr on 12/27/22.
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
    explicit Time_Object(sf::Int64 update_period_microseconds_);

    virtual void update() = 0; // Aktualizować last_update_time
    static std::list<Time_Object*>* get_all_time_objects_pointers();
    static void update_all_time_objets();

    virtual bool need_update(); // wiele instancji może nadpisywać tą funkcję

//    static std::string get_name() {return "";};
protected:
    // WAŻNE ! Aktualizować last update time !
    sf::Int64 update_period_microseconds;

    static sf::Clock clock;
    sf::Int64 last_update_time = 0;

    static sf::Time  main_clock_diff;
private:
    static std::list<Time_Object*> all_time_objects_pointers;

    const sf::Int64 default_update_period_microseconds = Configs::default_update_time;
};

#endif //INZYNIERKA_TIME_OBJECT_HPP
