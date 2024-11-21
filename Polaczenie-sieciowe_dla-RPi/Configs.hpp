//
// Created by wpr on 24.10.24.
//

#ifndef MAGISTERKA_CONFIGS_HPP
#define MAGISTERKA_CONFIGS_HPP

#include <SFML/Config.hpp>
#include <SFML/Graphics/Color.hpp>
#include <opencv4/opencv2/core.hpp>
#include <map>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

enum class Axes {
    x, y, z
};

static int get_axi_nr(Axes axis) {
    switch (axis) {
        case Axes::x:
            return 0;
        case Axes::y:
            return 1;
        case Axes::z:
            return 2;
        default:
            return -1;
    }
}

namespace Configs{
    static const sf::Time default_update_time = sf::milliseconds(50);

    static const sf::Time camera_update_period = sf::milliseconds(50);
    static const sf::Time max_camera_update_period_error = sf::milliseconds(20);

    static const sf::Time main_loop_time = sf::milliseconds(1);
    static const double max_intersection_size_comparison = 10;

    static const bool is_binarization_relative = false;
    static const int binarization_threshold = 25;

    static const bool is_in_2d_field_tested = true;
    static const bool is_3d_size_correct_test = true;


    namespace GUI_layout{
        static const sf::Vector2f default_small_window_size = sf::Vector2f(360, 230);

        static const sf::Vector2i render_window_size(1200,  800);
        static const std::string render_window_name("Inzynierka");

        static const bool is_projection_calculator_displayed = false;

        static const sf::FloatRect main_button_field(10, 10, 350, 100);
        static const sf::Color main_button_field_color = sf::Color::White;

        static const sf::Vector2f camera_view_window_pos(400, 30);
        static const sf::Vector2f binarization_window_pos(camera_view_window_pos.x + default_small_window_size.x + 10,
                                                          camera_view_window_pos.y);
        static const sf::Vector2f rays_window_pos(0, 0);

        static const sf::FloatRect first_connection_button_field(main_button_field.left,
                                                                 main_button_field.top + main_button_field.height + 10,
                                                                 main_button_field.width,
                                                                 main_button_field.height);
        static const sf::Color first_connection_button_field_color = sf::Color::Cyan;

        static const sf::FloatRect received_rays_field(10,
                                              first_connection_button_field.top + first_connection_button_field.height + 10,
                                                       350,
                                                       120);
        static const sf::Vector2f received_rays_zero_point(50, received_rays_field.width / 4);
        static const std::pair<Axes, Axes> received_rays_axes(Axes::z, Axes::x);
    }

    namespace Big_windows_parameters{
        struct Displayed_window_configs{
            sf::Vector2f window_size;
            sf::Vector2f window_pos;

            cv::Vec2d area_size;    // mm
            cv::Vec2d area_zero_point;  // mm

            std::pair<Axes, Axes> axes;
        };

        static std::vector<Displayed_window_configs> displayed_big_window_list{
                {{560, 410},
                 {GUI_layout::main_button_field.left, 370},

                 {3000, 3000},  // mm
                 {1500, 1500},

                 {Axes::z, Axes::x}
                 },
                {{570, 410},
                 {600, 370},

                 {3000, 3000},
                 {1500, 1500},

                 {Axes::z, Axes::y}
                }
        };

        static const sf::Time displayed_time = sf::milliseconds(2000);
        static const cv::Vec2d size_meters = cv::Vec2d(20, 10);
        static const cv::Vec2d zero_point_meters = cv::Vec2d(10, 5);

        static const float detection_radius_ratio = 0.000001;
        static const sf::Color detection_default_circle_color = sf::Color::Magenta;
        static const float detection_outline_thickness = 1.5;

        static const float position_ratio = 8;

//        static Axes first_axle = Axes::x;
//        static Axes second_axle = Axes::y;

        static const bool is_const_size = true;
        static const float default_object_size = 10;

        static const bool is_displayed_verified_only = true;

        static const float grid_span = 500;    // mm
        static const sf::Color grid_span_zero_point_color = sf::Color::Red;
        static const sf::Color grid_span_default_color(120, 120, 120);
    }

    enum class computers_enum{
        dell,
        hp
    };

    static const computers_enum local_computer = computers_enum::dell;
    static const computers_enum remote_computer = computers_enum::hp;

    static std::map<computers_enum, std::string> hw_folders_name{
            {computers_enum::dell, "Dell"},
            {computers_enum::hp, "Hp"},
    };

    struct camera_parameters_struct{
        const cv::Vec2d camera_size_mm;
        const cv::Vec2i camera_size_pixels;
    };

    static std::map<computers_enum, camera_parameters_struct> camera_params{
            {computers_enum::dell,{
                {
                    3.2, 2.4    //mm
                    },{
                    640, 480
                    }
                }
            },
            {computers_enum::hp, {
                {
                    3.2, 2.4    //mm
                    },{
                    640, 480
                    }
                }
            }
    };

    static cv::Vec2d millimeter_to_pixel(computers_enum computer_type, const cv::Vec2d& position_millimeter) {
        double pixel_to_millimeter_ratio_x = static_cast<double>(camera_params[computer_type].camera_size_pixels[0])
                                             / camera_params[computer_type].camera_size_mm[0];
        double pixel_to_millimeter_ratio_y = static_cast<double>(camera_params[computer_type].camera_size_pixels[1])
                                             / camera_params[computer_type].camera_size_mm[1];

        return {
                pixel_to_millimeter_ratio_x * position_millimeter[0],
                pixel_to_millimeter_ratio_y * position_millimeter[1]
        };
    }

    namespace Object_tracker{
        static const double max_size_proportion = 2;
        static const double max_distance = 10;
        static const int min_detect_number = 3;
        static const sf::Time remove_time = sf::seconds(1);
    }
}

#endif //MAGISTERKA_CONFIGS_HPP
