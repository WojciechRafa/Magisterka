//
// Created by wr on 12/17/22.
//

#include "Time_Object.hpp"

#include <iostream>

std::list<Time_Object*> Time_Object::all_time_objects_pointers;
sf::Clock Time_Object::clock;

Time_Object::Time_Object() {
    last_update_time = clock.getElapsedTime();
    all_time_objects_pointers.push_back(this);
    update_period = default_update_period_microseconds;
}

Time_Object::Time_Object(sf::Time update_period_):
        update_period(update_period_)
{
    last_update_time = clock.getElapsedTime();

    all_time_objects_pointers.push_back(this);
}

Time_Object::~Time_Object() {
    auto this_iterator = std::find(all_time_objects_pointers.begin(), all_time_objects_pointers.end(), this);

    if(this_iterator == all_time_objects_pointers.end()){
        std::cout<<this <<" wasn't found in list"<< std::endl;
    }else{
        all_time_objects_pointers.erase(this_iterator);
    }
}


std::list<Time_Object *>* Time_Object::get_all_time_objects_pointers() {
    return & all_time_objects_pointers;
}

bool Time_Object::need_update() {
    return clock.getElapsedTime() - last_update_time > update_period;
}

void Time_Object::update_all_time_objets() {
    static std::map<std::string, sf::Time> name_to_whole_time;
    for(auto time_object: all_time_objects_pointers){
        if(time_object->need_update()){
            sf::Time time_begin = clock.getElapsedTime();
            time_object->update();
            sf::Time time_end = clock.getElapsedTime();

            sf::Time time_diff = time_end - time_begin;
            name_to_whole_time[time_object->get_name()] += time_diff;
        }
    }
}

sf::Clock &Time_Object::get_clock() {
    return clock;
}

std::string Time_Object::get_name() {
    return "Not defined";
}
