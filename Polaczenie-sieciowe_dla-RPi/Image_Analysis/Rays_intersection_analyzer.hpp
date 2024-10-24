//
// Created by wpr on 29.09.24.
//

#ifndef MAGISTERKA_RAYS_INTERSECTION_ANALYZER_HPP
#define MAGISTERKA_RAYS_INTERSECTION_ANALYZER_HPP

#include <vector>
#include "Frame_parameters.hpp"
#include "Objects_tracker.hpp"
#include "../Time_Object.hpp"
#include "list"

class Rays_intersection_analyzer: public Time_Object{
public:
    void add_projection(std::unique_ptr<Frame_parameters> params);

    void update() override;
private:
    sf::Int64 last_rays_source_update;
    std::list<std::unique_ptr<Frame_parameters>> objets_parameters_list;
    Objects_tracker* tracker_ptr;

    using Potential_objets = std::vector<std::tuple<cv::Vec2d, cv::Vec2d, double>>;

    std::map<std::pair<Rays_source*, sf::Int64>,
        std::tuple<Frame_parameters*, Frame_parameters*, std::unique_ptr<Potential_objets>>> objets_pairs_of_same_source;

    std::vector<std::tuple<sf::Int64, Rays_source*, Rays_source*>> already_checked_intersections;


    // function return list of potential objects as list of tuples contained, begin pos, end pos and size as double
//    void get_potential_detected_object(std::vector<std::tuple<cv::Vec3d, cv::Vec3d, double>>& result,
//                                       std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>> rays_begin,
//                                       std::vector<std::tuple<cv::Vec3d, cv::Vec3d, cv::Vec3d>> rays_end,
//                                       sf::Int64 time_diff,
//                                       cv::Mat internal_matrix);
//    static void interpolate_position_vector(std::vector<std::tuple<cv::Vec2d, cv::Vec2d, double>>& pos_vector,
//                                            sf::Int64 time_begin,
//                                            sf::Int64 time_end,
//                                            sf::Int64 time_interpolated,
//                                            bool is_first_vector_changed);
    static void interpolate_position_vector(std::vector<std::tuple<cv::Vec2d, cv::Vec2d, double>>& pos_vector,
                                            sf::Int64 time_begin,
                                            sf::Int64 time_end,
                                            sf::Int64 time_interpolated,
                                            bool is_first_vector_changed);
    static void calculate_double_intersections( std::vector<std::tuple<cv::Vec3d, cv::Vec3d, double>>& result, // position_time_begin, postion_time_end, size
                                                std::vector<std::tuple<cv::Vec2d, cv::Vec2d, double>>& first_projections,
                                                Rays_source& first_source,
                                                std::vector<std::tuple<cv::Vec2d, cv::Vec2d, double>>& second_projections,
                                                Rays_source& second_source);

    static void calculate_single_intersections( std::vector<cv::Vec3d>& result_pos,
                                                std::vector<double>& result_size,
                                                std::vector<bool> &result_is_correct,
                                                std::vector<cv::Vec2d>& first_2d_pos,
                                                std::vector<double>& first_2d_size,
                                                Rays_source& first_source,
                                                std::vector<cv::Vec2d>& second_2d_pos,
                                                std::vector<double>& second_2d_size,
                                                Rays_source& second_source);
};

#endif //MAGISTERKA_RAYS_INTERSECTION_ANALYZER_HPP
