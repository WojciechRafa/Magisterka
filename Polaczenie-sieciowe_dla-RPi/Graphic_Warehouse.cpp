//
// Created by wpr on 11.12.22.
//

#include <iostream>
#include "Graphic_Warehouse.hpp"

Graphic_Warehouse::Graphic_Warehouse(std::string folder_path) {
    for(const auto& file_name: file_name_list){
        sf::Texture texture;
        std::string full_path;
        full_path += folder_path;
        full_path += "/";
        full_path += file_name;
        full_path += ".png";
        texture.loadFromFile(full_path);
        texture_list.push_back(texture);
    }

}

sf::Texture &Graphic_Warehouse::get_texture(const std::string& name) {
    auto itr = std::find(file_name_list.begin(), file_name_list.end(), name);
    if(itr == file_name_list.end())
        std::cerr<<"Nie znazleciono pliku "<< name;

    auto pos = std::distance(file_name_list.begin(), itr);

    return texture_list[pos];
}
