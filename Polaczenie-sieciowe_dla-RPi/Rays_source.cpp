//
// Created by wpr on 29.10.24.
//

#include "Rays_source.hpp"

Rays_source::Rays_source(Rays_intersection_analyzer* rays_intersection_analyzer_ptr_):
    rays_intersection_analyzer_ptr(rays_intersection_analyzer_ptr_){};

cv::Mat &Rays_source::get_outside_matrix() {
    return outside_matrix;
}

cv::Mat &Rays_source::get_inside_matrix() {
    return inside_matrix;
}

void Rays_source::set_rays_intersection_analyzer(Rays_intersection_analyzer *rays_intersection_analyzer_ptr_) {
    rays_intersection_analyzer_ptr = rays_intersection_analyzer_ptr_;
}
