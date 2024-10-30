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
    explicit Rays_source(Rays_intersection_analyzer* rays_intersection_analyzer_ptr_);
    cv::Mat& get_outside_matrix();
    cv::Mat& get_inside_matrix();

    void set_rays_intersection_analyzer(Rays_intersection_analyzer* rays_intersection_analyzer_ptr_);
protected:
    Rays_intersection_analyzer* rays_intersection_analyzer_ptr = nullptr;
    cv::Mat inside_matrix;
    cv::Mat outside_matrix;
};

#endif //MAGISTERKA_RAYS_SOURCE_HPP
