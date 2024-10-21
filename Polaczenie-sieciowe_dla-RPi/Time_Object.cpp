#include "Time_Object.hpp"

#include <iostream>

std::list<Time_Object*> Time_Object::all_time_objects_pointers;


Time_Object::Time_Object() {
    clock.restart();
    last_update_time = clock.getElapsedTime().asMicroseconds();

    all_time_objects_pointers.push_back(this);
}

Time_Object::Time_Object(sf::Int64 update_period_microseconds_):
        update_period_microseconds(update_period_microseconds_)
{
    clock.restart();
    last_update_time = clock.getElapsedTime().asMicroseconds();

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
    return clock.getElapsedTime().asMicroseconds() - last_update_time > update_period_microseconds;
}

void Time_Object::update_all_time_objets() {
    for(const auto time_object: all_time_objects_pointers){
        if(time_object->need_update()){
            if(not time_object->get_name().empty())
                std::cout << time_object->get_name() << std::endl;
            time_object->update();
        }
    }
}

