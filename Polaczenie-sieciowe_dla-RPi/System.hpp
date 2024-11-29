//
// Created by wr on 12/12/22.
//

#ifndef INZYNIERKA_SYSTEM_HPP
#define INZYNIERKA_SYSTEM_HPP

#include "GUI/Buttons_Field.hpp"
#include "GUI/Graphic_Warehouse.hpp"
#include "main_functions.hpp"
#include "Connection/Connection.hpp"
#include "GUI/Graphic_Manager.hpp"

#include "GUI/Small_window.hpp"
#include "Image_Analysis/Image_source.hpp"
#include "Image_Analysis/Binarization.hpp"
#include "Image_Analysis/Projection_image_calculator.hpp"
#include "Image_Analysis/Rays_intersection_analyzer.hpp"
#include "GUI/Detected_objects_display.hpp"

class System {
public:
    explicit System();

    bool update();
private:
    sf::Time update_period;
    bool execute_button_message(Button::Button_Message message);

    sf::RenderWindow window;

    std::vector<std::unique_ptr<Connection>> connection_list;

    sf::Event event{};
    Graphic_Warehouse graphic_warehouse;

    Graphic_Manager graphic;

    std::unique_ptr<Buttons_Field>  create_button_field_to_connection_with_camera_and_custom_data(sf::Vector2f button_field_pos = sf::Vector2f(10, 120));
    std::unique_ptr<Buttons_Field>  create_button_field_to_connection_with_rays(sf::Vector2f button_field_pos = sf::Vector2f(10, 120));

    Image_source image_source;
    Binarization binarization;

    std::shared_ptr<std::pair<sf::Time, cv::Mat>> raw_picture_with_main_time
        = std::make_shared<std::pair<sf::Time, cv::Mat>>();
    std::shared_ptr<cv::Mat> binarized_picture = std::make_shared<cv::Mat>();

    Small_window raw_picture_window;
    Small_window binarized_picture_window;
    Small_window projections_window;

    std::vector<Small_window> detected_objects_windows;

    std::shared_ptr<Binarization::Binarized_parameters> bin_parameters =
            std::make_shared<Binarization::Binarized_parameters>();

    std::vector<std::unique_ptr<sf::Drawable>> projections = std::vector<std::unique_ptr<sf::Drawable>>();

    Projection_image_calculator projection_calculator;
    Rays_intersection_analyzer rays_intersection_analyzer;
    Objects_tracker objects_tracker;
    std::vector<Detected_objects_display> detected_objects_displayer_list;

    sf::Clock& clock;
};


#endif //INZYNIERKA_SYSTEM_HPP
