//
// Created by wpr on 21.08.24.
//

#ifndef MAGISTERKA_BINARIZATION_HPP
#define MAGISTERKA_BINARIZATION_HPP

#include <opencv2/opencv.hpp>
#include "Time_Object.hpp"

class Binarization: public Time_Object{
public:
    struct Binarized_parameters{
        int numb_labels;
        cv::Mat stats;
        cv::Mat centroids;
    };

    Binarization(int change_time_ = 50000);
    void update() override;

    void set_input_image(std::shared_ptr<cv::Mat> image_);
    void set_binarized_image(std::shared_ptr<cv::Mat> image_);

//    cv::Mat* get_stats();
//    cv::Mat* get_centroids();
//    void set_stats(std::shared_ptr<cv::Mat> stats_);
//    void set_centroids(std::shared_ptr<cv::Mat> centroids_);
    void set_parameters(std::shared_ptr<Binarized_parameters> parameters_);

private:
    std::shared_ptr<cv::Mat> input_image = nullptr;
    std::shared_ptr<cv::Mat> binarized_image_result = nullptr;

//    std::unique_ptr<cv::Mat> binarized_image = nullptr;
    std::unique_ptr<cv::Mat> image_m1 = nullptr;
    std::unique_ptr<cv::Mat> image_m2 = nullptr;


    std::shared_ptr<Binarized_parameters> parameters = nullptr;
//    std::shared_ptr<cv::Mat> stats = nullptr;
//    std::shared_ptr<cv::Mat> centroids = nullptr;
};


#endif //MAGISTERKA_BINARIZATION_HPP
