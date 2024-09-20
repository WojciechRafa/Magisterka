//
// Created by wr on 12/12/22.
//

#ifndef INZYNIERKA_SYSTEM_HPP
#define INZYNIERKA_SYSTEM_HPP

#include "Window.hpp"
#include "Buttons_Field.hpp"
#include "Graphic_Warehouse.hpp"
#include "main_functions.hpp"
#include "Connection.hpp"
#include "Graphic_Manager.hpp"

#include "Small_window.hpp"
#include "Image_source_typ.hpp"
#include "Binarization.hpp"
#include "Projection_image_calculator.hpp"

class System {
public:
    explicit System(sf::Int64 update_period_microseconds_ = 1000); // domyślne maksymalne tempo aktualizacji 1ms

    bool update();
private:
    bool execute_button_message(Button::Button_Message message); // zwraca informacje o tym czy należy zamknąć program

    Window window;

    std::vector<std::unique_ptr<Connection>> connection_list;

    sf::Event event{};
    Graphic_Warehouse graphic_warehouse;

    Graphic_Manager graphic;

    sf::Int64 update_period_microseconds;

    std::unique_ptr<Buttons_Field>  create_button_field_to_connection_with_camera_and_custom_data(sf::Vector2f button_field_pos = sf::Vector2f(10, 120));
    std::unique_ptr<Buttons_Field>  create_button_field_to_connection_with_rays(sf::Vector2f button_field_pos = sf::Vector2f(10, 120));

    // camera data windows
    const sf::Vector2f standard_window_size = sf::Vector2f(300, 200);

    Image_source image_source;
    Binarization binarization;

    std::shared_ptr<cv::Mat> raw_picture = std::make_shared<cv::Mat>();
    std::shared_ptr<cv::Mat> binarized_picture = std::make_shared<cv::Mat>();

    Small_window raw_picture_window;
    Small_window binarized_picture_window;
    Small_window projections_window;

    std::shared_ptr<Binarization::Binarized_parameters> bin_parameters =
            std::make_shared<Binarization::Binarized_parameters>();

    std::vector<std::unique_ptr<sf::Shape>> projections = std::vector<std::unique_ptr<sf::Shape>>();

    Projection_image_calculator projection_calculator;

    sf::Clock clock;
};


#endif //INZYNIERKA_SYSTEM_HPP
