//
// Created by wpr on 22.08.24.
//

#ifndef MAGISTERKA_PROJECTION_IMAGE_CALCULATOR_HPP
#define MAGISTERKA_PROJECTION_IMAGE_CALCULATOR_HPP

#include "../Time_Object.hpp"
#include <opencv2/opencv.hpp>
#include "SFML/Graphics.hpp"
#include "Binarization.hpp"
#include "../Network/Sended_struct.hpp"

class Projection_image_calculator: public Time_Object{
public:

    Projection_image_calculator(
            Axes axis_a_,
            Axes axis_b_,
            sf::Vector2f window_pos_ = sf::Vector2f(0, 0),
            sf::Vector2f window_size_ = sf::Vector2f(0, 0),
            sf::Vector2f zero_point_pos_ = sf::Vector2f(0, 0)
);
    void update() override;

    void set_parameters(std::shared_ptr<Binarization::Binarized_parameters> parameters_);

    void set_additional_drawable_ptr(std::vector<std::unique_ptr<sf::Drawable>>* additional_graphic_);

private:
    std::shared_ptr<Binarization::Binarized_parameters> parameters;

    Axes axis_a;
    Axes axis_b;

    std::vector<std::unique_ptr<sf::Drawable>>* drawable_list = nullptr;

    cv::Mat internal_matrix;

    static cv::Vec3d compute_3D_line(const cv::Mat& intrinsicMatrix, const cv::Point2d& imagePoint);

    sf::Vector2f window_pos;
    sf::Vector2f output_window_size;

    float corners_angle[4]{};

    sf::Vector2f output_zero_point_pos;

    sf::Vector2f find_intersection(double axis_a_value, double axis_b_value);

    std::unique_ptr<sf::RectangleShape> get_ray(sf::Vector2f intersection, sf::Color color, float thickness = 3);

    cv::Vec3d multiple_internal(cv::Vec2d& image_params);
};


#endif //MAGISTERKA_PROJECTION_IMAGE_CALCULATOR_HPP
