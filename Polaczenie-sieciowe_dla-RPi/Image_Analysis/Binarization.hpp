//
// Created by wpr on 21.08.24.
//

#ifndef MAGISTERKA_BINARIZATION_HPP
#define MAGISTERKA_BINARIZATION_HPP

#include <opencv2/opencv.hpp>
#include "../Time_Object.hpp"

class Binarization: public Time_Object{
public:
    struct Binarized_parameters{
        int numb_labels;
        cv::Mat stats;
        cv::Mat centroids;
    };

    explicit Binarization(bool is_relative_, int change_time_ = 50000);
    void update() override;

    void set_input_image(std::shared_ptr<cv::Mat> image_);
    void set_binarized_image(std::shared_ptr<cv::Mat> image_);

    void set_reference_image();

//    cv::Mat* get_stats();
//    cv::Mat* get_centroids();
//    void set_stats(std::shared_ptr<cv::Mat> stats_);
//    void set_centroids(std::shared_ptr<cv::Mat> centroids_);
    void set_parameters(std::shared_ptr<Binarized_parameters> parameters_);

private:
    bool is_relative;

    std::shared_ptr<cv::Mat> input_image = nullptr;
    std::shared_ptr<cv::Mat> binarized_image_result = nullptr;

    std::unique_ptr<cv::Mat> image_m1 = nullptr;    // used by relative_update()
    std::unique_ptr<cv::Mat> image_m2 = nullptr;    // used by relative_update()

    std::unique_ptr<cv::Mat> reference_image = nullptr;


    std::shared_ptr<Binarized_parameters> parameters = nullptr;


    void absolute_update();
    void relative_update();
};


#endif //MAGISTERKA_BINARIZATION_HPP
