//
// Created by wpr on 16.08.24.
//

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

#ifndef INZYNIERKA_GRAPHIC_WAREHOUSE_HPP
#define INZYNIERKA_GRAPHIC_WAREHOUSE_HPP

// open only ".png" files
class Graphic_Warehouse {
public:
    explicit Graphic_Warehouse(std::string folder_path);

    sf::Texture& get_texture(const std::string& name);
private:
    std::vector<std::string> file_name_list =
            {"Background",
             "Cancel",
             "Set_reference_image"
            };
    std::vector<sf::Texture> texture_list;
};


#endif //INZYNIERKA_GRAPHIC_WAREHOUSE_HPP