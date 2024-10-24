//
// Created by wr on 12/17/22.
//

#include "Time_Object.hpp"

std::vector<Time_Object*> Time_Object::all_time_objects_pointers;
sf::Int64 Time_Object::default_update_period_microseconds;

Time_Object::Time_Object():
    update_period_microseconds(default_update_period_microseconds)
{
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

std::vector<Time_Object *>* Time_Object::get_all_time_objects_pointers() {
    return & all_time_objects_pointers;
}

bool Time_Object::need_update() {
    return clock.getElapsedTime().asMicroseconds() - last_update_time > update_period_microseconds;
}

void Time_Object::update_all_time_objets() {
    for(auto time_object: all_time_objects_pointers){
        if(time_object->need_update()){
            time_object->update();
        }
    }
}

Time_Object::~Time_Object() {
    auto this_iterator = std::find(all_time_objects_pointers.begin(), all_time_objects_pointers.end(), this);

    auto ptr_value = *this_iterator;
    if(ptr_value != nullptr)
        all_time_objects_pointers.erase(this_iterator);
}


