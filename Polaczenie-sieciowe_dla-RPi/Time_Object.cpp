#include "Time_Object.hpp"

#include <iostream>

std::list<Time_Object*> Time_Object::all_time_objects_pointers;
sf::Clock Time_Object::clock;
sf::Time Time_Object::main_clock_diff;

Time_Object::Time_Object() {
    clock.restart();
    last_update_time = clock.getElapsedTime();

    all_time_objects_pointers.push_back(this);

    update_period = default_update_period_microseconds;
}

Time_Object::Time_Object(sf::Time update_period_):
        update_period(update_period_)
{
    clock.restart();
    last_update_time = clock.getElapsedTime();

    all_time_objects_pointers.push_back(this);
}

Time_Object::~Time_Object()
{
    const auto this_iterator = std::find(all_time_objects_pointers.begin(), all_time_objects_pointers.end(), this);
    all_time_objects_pointers.erase(this_iterator);
}


std::list<Time_Object *>* Time_Object::get_all_time_objects_pointers() {
    return & all_time_objects_pointers;
}

bool Time_Object::need_update() {
    return clock.getElapsedTime() - last_update_time > update_period;
}

void Time_Object::update_all_time_objets() {
    for(Time_Object* time_object: all_time_objects_pointers){
        if(time_object->need_update()){
            time_object->update();
        }
    }
}

