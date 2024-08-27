//
// Created by wpr on 21.08.24.
//

#include "Binarization.hpp"

void get_binary_diff(cv:: Mat& frame_1, cv:: Mat& frame_2, cv::Mat& result){
    cv::Mat diff, gray_diff;

    cv::absdiff(frame_1, frame_2, diff);
    cv::cvtColor(diff, gray_diff, cv::COLOR_BGR2GRAY);
    cv::threshold(gray_diff, result, 30, 255, cv::THRESH_BINARY);
}

void Binarization::update() {
    parameters->numb_labels = 0;

    if(input_image == nullptr or input_image->empty()){
        last_update_time = clock.getElapsedTime().asMicroseconds();
        return;
    }

    if(image_m1 != nullptr and image_m2 != nullptr){
        cv::Mat threshold_diff_1, threshold_diff_2;

        get_binary_diff(*input_image, *image_m1, threshold_diff_1);
        get_binary_diff(*input_image, *image_m2, threshold_diff_2);

        cv::Mat and_result;
        cv::bitwise_and(threshold_diff_1, threshold_diff_2, and_result);

        int kernelSize = 5;  // Can be adjusted
        cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelSize, kernelSize));

        cv::Mat openedImage;

        cv::morphologyEx(and_result, *binarized_image_result, cv::MORPH_OPEN, element);

        cv::Mat labels;
        cv::Mat a, b;

        parameters->numb_labels = cv::connectedComponentsWithStats(and_result, labels, parameters->stats, parameters->centroids);
        parameters->numb_labels --; // first label is background, so it should be missed.
    }
    if (image_m1 != nullptr)
        image_m2 = std::move(image_m1);
    image_m1 = std::make_unique<cv::Mat>(input_image->clone());

    last_update_time = clock.getElapsedTime().asMicroseconds();
}

void Binarization::set_input_image(std::shared_ptr<cv::Mat> image_) {
    input_image = std::move(image_);
}

void Binarization::set_binarized_image(std::shared_ptr<cv::Mat> image_) {
    binarized_image_result = std::move(image_);
}

Binarization::Binarization(int change_time_):
    Time_Object(change_time_)
{}

void Binarization::set_parameters(std::shared_ptr<Binarized_parameters> parameters_) {
    parameters = std::move(parameters_);
}
