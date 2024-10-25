//
// Created by wpr on 30.09.24.
//

#ifndef MAGISTERKA_RAYS_SOURCE_HPP
#define MAGISTERKA_RAYS_SOURCE_HPP

#include <opencv2/core/mat.hpp>
#include <utility>

class Rays_source{
public:
    explicit Rays_source(cv::Mat outside_matrix_ = cv::Mat::zeros(3, 4, CV_64F)): outside_matrix(std::move(outside_matrix_)){};
    cv::Mat& get_outside_matrix(){return outside_matrix;};
    cv::Mat& get_inside_matrix(){return inside_matrix;};
protected:
    cv::Mat inside_matrix;
    cv::Mat outside_matrix;
};

#endif //MAGISTERKA_RAYS_SOURCE_HPP
