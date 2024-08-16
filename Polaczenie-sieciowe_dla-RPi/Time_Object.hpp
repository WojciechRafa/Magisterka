//
// Created by wr on 12/17/22.
//

#ifndef INZYNIERKA_TIME_OBJECT_HPP
#define INZYNIERKA_TIME_OBJECT_HPP

#include <SFML/System/Clock.hpp>
#include <list>
#include <memory>
// klasa będzie wykorzystywana do obsługi wszystkich obiektów które muszą być aktualizowane w regularnych okresach czas

class Time_Object {
public:
    Time_Object();
    explicit Time_Object(sf::Int64 update_period_microseconds_);

    virtual void update() = 0; // Aktualizować last_update_time
    std::list<Time_Object&>* get_all_contaneted_time_object();

    bool need_update(); // wiele instancji może nadpisywać tą funkcję
protected:
    // WAŻNE ! Aktualizować last update time !
    sf::Int64 update_period_microseconds = 100000;

    sf::Clock clock; // TODO zmiana tego w static (problemy są z tym)
    sf::Int64 last_update_time = 0;
};


#endif //INZYNIERKA_TIME_OBJECT_HPP
