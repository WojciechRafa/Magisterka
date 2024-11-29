//
// Created by wr on 12/17/22.
//

#ifndef INZYNIERKA_TIME_OBJECT_HPP
#define INZYNIERKA_TIME_OBJECT_HPP

#include <SFML/System/Clock.hpp>
#include <list>
#include <memory>
#include "Configs.hpp"

class Time_Object {
public:
    Time_Object();
    ~Time_Object();
    explicit Time_Object(sf::Time update_period_);

    Time_Object(Time_Object&&) noexcept = default;
    Time_Object& operator=(Time_Object&&) noexcept = default;


    virtual void update() = 0; // Aktualizować last_update_time
    static std::list<Time_Object*>* get_all_time_objects_pointers();
    static void update_all_time_objets();

    virtual bool need_update(); // wiele instancji może nadpisywać tą funkcję

    static sf::Clock& get_clock();

    virtual std::string get_name();
protected:
    // WAŻNE ! Aktualizować last update time !
    sf::Time update_period;

    static sf::Clock clock;
    sf::Time last_update_time = sf::milliseconds(0);
private:
    static std::list<Time_Object*> all_time_objects_pointers;
};

#endif //INZYNIERKA_TIME_OBJECT_HPP
