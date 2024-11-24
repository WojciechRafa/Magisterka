//
// Created by wpr on 29.10.24.
//

#include "Rays_source.hpp"

Rays_source::Rays_source(Rays_intersection_analyzer* rays_intersection_analyzer_ptr_, Configs::computers_enum computer_type_):
    rays_intersection_analyzer_ptr(rays_intersection_analyzer_ptr_),
    computer_type(computer_type_),
    camera_params(Configs::camera_params[computer_type])
    {};

cv::Mat &Rays_source::get_external_matrix() {
    return external_matrix;
}

cv::Mat &Rays_source::get_internal_matrix() {
    return internal_matrix;
}

Configs::camera_parameters *Rays_source::get_camera_params() {
    return &camera_params;
}
