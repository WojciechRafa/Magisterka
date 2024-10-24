//
// Created by wpr on 22.08.24.
//

#ifndef MAGISTERKA_PROJECTION_IMAGE_CALCULATOR_HPP
#define MAGISTERKA_PROJECTION_IMAGE_CALCULATOR_HPP

#include <opencv2/opencv.hpp>
#include "SFML/Graphics.hpp"
#include "Binarization.hpp"
#include "../Time_Object.hpp"
#include "../Rays_source.hpp"

class Projection_image_calculator: public Time_Object, public Rays_source{
public:
    enum class axes{
        x, y, z
    };

    Projection_image_calculator(axes axis_a_,
                                axes axis_b_,
                                sf::Vector2f window_pos_ = sf::Vector2f(0, 0),
                                sf::Vector2f window_size_ = sf::Vector2f(0, 0),
                                sf::Vector2f zero_point_pos_ = sf::Vector2f(0, 0),

                                bool are_rays_from_slave_ = false,
                                std::vector<std::tuple<cv::Vec2d, cv::Vec2d, cv::Vec2d>>* received_parameters_ = {},

                                cv::Mat internal_matrix_ = cv::Mat::zeros(3, 3, CV_8UC1),
                                cv::Mat external_matrix_ = cv::Mat::zeros(3, 4, CV_8UC1),
                                int change_time_ = 50000);
    void update() override;

    void set_parameters(std::shared_ptr<Binarization::Binarized_parameters> parameters_);


    void set_additional_graphic(std::vector<std::unique_ptr<sf::Shape>>* additional_graphic_);

private:
    std::shared_ptr<Binarization::Binarized_parameters> parameters;

    axes axis_a;
    axes axis_b;

    std::vector<std::unique_ptr<sf::Shape>>* projections_list = nullptr;

    static cv::Mat load_camera_matrix(const std::string& filePath);
    cv::Mat internal_parameters;

    static cv::Vec3d compute_3D_line(const cv::Mat& intrinsicMatrix, const cv::Point2d& imagePoint);

    sf::Vector2f window_pos;
    sf::Vector2f output_window_size;

    float corners_angle[4]{};

    sf::Vector2f output_zero_point_pos;

    sf::Vector2f find_intersection(double axis_a_value, double axis_b_value);

    std::unique_ptr<sf::RectangleShape> get_ray(sf::Vector2f intersection, sf::Color color, float thickness = 3);

    static int get_axi_nr(axes);

    bool are_rays_from_slave = false;
    std::vector<std::tuple<cv::Vec2d, cv::Vec2d, cv::Vec2d>>*  received_parameters;

    cv::Vec3d multiple_internal(cv::Vec2d& image_params);
};


#endif //MAGISTERKA_PROJECTION_IMAGE_CALCULATOR_HPP
