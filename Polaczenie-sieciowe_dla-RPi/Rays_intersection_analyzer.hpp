//
// Created by wpr on 29.09.24.
//

#ifndef MAGISTERKA_RAYS_INTERSECTION_ANALYZER_HPP
#define MAGISTERKA_RAYS_INTERSECTION_ANALYZER_HPP

#include <vector>
#include "Rays_parameters.hpp"
#include "Objects_tracker.hpp"
#include "Time_Object.hpp"

class Rays_intersection_analyzer: public Time_Object{
public:
    void add_projection(std::unique_ptr<Rays_parameters> params);

    void update() override;
private:
    sf::Int64 last_rays_source_update;
    std::list<std::unique_ptr<Rays_parameters>> rays_parameters_list;
    Objects_tracker* tracker_ptr;

    using Potential_objets = std::vector<std::tuple<cv::Vec3d, cv::Vec3d, double>>;

    std::map<std::pair<Rays_source*, sf::Int64>,
        std::tuple<Rays_parameters*, Rays_parameters*, std::unique_ptr<Potential_objets>>> rays_pairs_of_same_source;

    std::vector<std::tuple<sf::Int64, Rays_source*, Rays_source*>> already_checked_intersections;


    // function return list of potential objects as list of tuples contained, begin pos, end pos and size as double
//    void get_potential_detected_object(std::vector<std::tuple<cv::Vec3d, cv::Vec3d, double>>& result,
//                                       std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>> rays_begin,
//                                       std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>> rays_end,
//                                       sf::Int64 time_diff,
//                                       cv::Mat internal_matrix);

};

#endif //MAGISTERKA_RAYS_INTERSECTION_ANALYZER_HPP
