//
// Created by wpr on 22.08.24.
//

#ifndef MAGISTERKA_PROJECTION_IMAGE_CALCULATE_HPP
#define MAGISTERKA_PROJECTION_IMAGE_CALCULATE_HPP

#include "../Time_Object.hpp"
#include <opencv2/opencv.hpp>
#include "SFML/Graphics.hpp"
#include "Binarization.hpp"
#include "../Network/Sended_struct.hpp"

class Projection_image_calculate: public Time_Object{
public:
    enum class axes{
        x, y, z
    };

    Projection_image_calculate(axes axis_a_,
                               axes axis_b_,
                               sf::Vector2f window_pos_ = sf::Vector2f(0, 0),
                               sf::Vector2f window_size_ = sf::Vector2f(0, 0),
                               sf::Vector2f zero_point_pos_ = sf::Vector2f(0, 0),
                               int change_time_ = 50000);
    void update() override;

    void set_parameters(std::shared_ptr<Binarization::Binarized_parameters> parameters_);


    void set_additional_graphic(std::shared_ptr<std::vector<std::unique_ptr<sf::Shape>>> additional_graphic_);
    void set_sent_parameters_ptr(Sent_parameters* sent_parameters_);
    void set_vectors_list(std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>>* vectors_list_);

private:
    std::shared_ptr<Binarization::Binarized_parameters> parameters;

    axes axis_a;
    axes axis_b;

    std::shared_ptr<std::vector<std::unique_ptr<sf::Shape>>> projections = nullptr;
    Sent_parameters* sent_parameters = nullptr;
    std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>>* vectors_list = nullptr;

    cv::Mat internal_matrix;

    static cv::Vec3d compute_3D_line(const cv::Mat& intrinsicMatrix, const cv::Point2d& imagePoint);

    sf::Vector2f window_pos;
    sf::Vector2f output_window_size;

    float corners_angle[4];

    sf::Vector2f output_zero_point_pos;

    sf::Vector2f find_intersection(double axis_a_value, double axis_b_value);

    std::unique_ptr<sf::RectangleShape> get_ray(sf::Vector2f intersection, sf::Color color, float thickness = 3);

    static int get_axi_nr(axes);
};


#endif //MAGISTERKA_PROJECTION_IMAGE_CALCULATE_HPP
