//
// Created by wpr on 29.09.24.
//

#ifndef MAGISTERKA_RAYS_INTERSECTION_ANALYZER_HPP
#define MAGISTERKA_RAYS_INTERSECTION_ANALYZER_HPP

#include <vector>
#include "Frame_parameters.hpp"
#include "../Objects_tracker.hpp"
#include "../Time_Object.hpp"
//#include "../Rays_source.hpp"
#include "list"

class Rays_source;

class Rays_intersection_analyzer: public Time_Object{
public:
    Rays_intersection_analyzer();
    void add_projection(const std::shared_ptr<Frame_parameters>& new_params);
    void set_objects_tracker_ptr(Objects_tracker* objects_tracker_);

    void update();
private:
    std::map<sf::Time, std::vector<std::shared_ptr<Frame_parameters>>> objets_parameters_list_by_time;
    std::map<Rays_source*, sf::Time> last_update_time_of_ray_source;
    std::vector<std::tuple<sf::Time, Rays_source*, Rays_source*>> already_checked_intersections;
    Objects_tracker* objects_tracker_ptr = nullptr;

    std::vector<cv::Vec3d> result_pos;
    std::vector<double> result_size;

    static bool check_2d_projection(const cv::Mat& position_normalized,
                             const cv::Mat& outside_matrix,
                             const cv::Mat& inside_matrix_wide,
                             cv::Vec2d bb_pos_2d,
                             cv::Vec2d bb_size_2d);

    static bool check_size_comparison(  double& estimated_size,
                                        const cv::Mat &position_normalized,
                                        Rays_source first_ray_source,
                                        const cv::Vec2d& first_bb_size_2d,
                                        Rays_source& second_ray_source,
                                        const cv::Vec2d& second_bb_size_2d);

    static void calculate_intersections( std::vector<cv::Vec3d>& result_pos,
                                  std::vector<double>& result_size,
                                  Frame_parameters* first_frame_params,
                                  Frame_parameters* second_frame_params);

    static double get_estimated_3d_size(cv::Vec2d size_2d, double distance_3d, Rays_source& rays_source);
};

#endif //MAGISTERKA_RAYS_INTERSECTION_ANALYZER_HPP
