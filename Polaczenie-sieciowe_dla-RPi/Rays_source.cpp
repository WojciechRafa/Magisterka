//
// Created by wpr on 29.10.24.
//

#include "Rays_source.hpp"

Rays_source::Rays_source(Rays_intersection_analyzer* rays_intersection_analyzer_ptr_):
    rays_intersection_analyzer_ptr(rays_intersection_analyzer_ptr_){};

cv::Mat &Rays_source::get_external_matrix() {
    return external_matrix;
}

cv::Mat &Rays_source::get_internal_matrix() {
    return internal_matrix;
}

void Rays_source::set_rays_intersection_analyzer(Rays_intersection_analyzer *rays_intersection_analyzer_ptr_) {
    rays_intersection_analyzer_ptr = rays_intersection_analyzer_ptr_;
}
