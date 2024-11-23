//
// Created by wpr on 30.09.24.
//

#ifndef MAGISTERKA_RAYS_SOURCE_HPP
#define MAGISTERKA_RAYS_SOURCE_HPP

#include <opencv2/core/mat.hpp>
#include <utility>
#include "Image_Analysis/Rays_intersection_analyzer.hpp"
//class Frame_parameters;
//
//class Rays_intersection_analyzer{
//public:
//    void add_projection(std::unique_ptr<Frame_parameters> new_params);
//};

class Rays_source{
public:
    explicit Rays_source(Rays_intersection_analyzer* rays_intersection_analyzer_ptr_, Configs::computers_enum computer_type_);
    cv::Mat& get_internal_matrix();
    cv::Mat& get_external_matrix();

    Configs::camera_parameters_struct* get_camera_params();
protected:
    Rays_intersection_analyzer* rays_intersection_analyzer_ptr = nullptr;
    cv::Mat internal_matrix;
    cv::Mat external_matrix;

    Configs::computers_enum computer_type;
    Configs::camera_parameters_struct camera_params;
};

#endif //MAGISTERKA_RAYS_SOURCE_HPP