//
// Created by wpr on 11.12.22.
//
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

#ifndef INZYNIERKA_GRAPHIC_WAREHOUSE_HPP
#define INZYNIERKA_GRAPHIC_WAREHOUSE_HPP

// magazyn otwiera tylko pliki z listy file_name_list z dopiskiem .png
class Graphic_Warehouse {
public:
    explicit Graphic_Warehouse(std::string folder_path);

    sf::Texture& get_texture(const std::string& name);
private:
    std::vector<std::string> file_name_list =
            {"Background",
             "Connection_Camera_View",
             "Connection_Data_Exchange",
             "Plus",
             "Cancel",
             "Camera_view_deafult",
             "Broadcast",
            "20_fps",
            "x1",
            "x0_5"

            };
    std::vector<sf::Texture> texture_list;
};


#endif //INZYNIERKA_GRAPHIC_WAREHOUSE_HPP
