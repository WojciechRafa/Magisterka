//
// Created by wr on 12/17/22.
//

#ifndef INZYNIERKA_TIME_OBJECT_HPP
#define INZYNIERKA_TIME_OBJECT_HPP

#include <SFML/System/Clock.hpp>
//#include <list>
#include <vector>
#include <memory>
// klasa będzie wykorzystywana do obsługi wszystkich obiektów które muszą być aktualizowane w regularnych okresach czas

class Time_Object {
public:
    Time_Object();
    explicit Time_Object(sf::Int64 update_period_microseconds_);
    ~Time_Object();

    virtual void update() = 0; // Aktualizować last_update_time
    static std::vector<Time_Object*>* get_all_time_objects_pointers();
    static void update_all_time_objets();

    bool need_update(); // wiele instancji może nadpisywać tą funkcję

    static sf::Int64 default_update_period_microseconds;
protected:
    // WAŻNE ! Aktualizować last update time !
    sf::Int64 update_period_microseconds;

    sf::Clock clock; // TODO zmiana tego w static (problemy są z tym)
    sf::Int64 last_update_time = 0;
private:
    static std::vector<Time_Object*> all_time_objects_pointers;
};


#endif //INZYNIERKA_TIME_OBJECT_HPP
