//
// Created by rodzi on 23.10.2021.
//

#ifndef SR_1_1_MAIN_FUNCTIONS_HPP
#define SR_1_1_MAIN_FUNCTIONS_HPP

#include <opencv2/opencv.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <fstream>

inline void write_comunicate_sockte_status(sf::Socket::Status status){
    switch (status){
        case sf::Socket::Disconnected:
            std::cout<<"Nie polaczono"<<std::endl;
            break;
        case sf::Socket::Error :
            std::cout<<"Blad"<<std::endl;
            break;
        case sf::Socket::NotReady :
            std::cout<<"Nie gotowy"<<std::endl;
            break;
        case sf::Socket::Partial :
            std::cout<<"Czesciowo polaczony"<<std::endl;
            break;
        case sf::Socket::Done:
            std::cout<<"Polacznie poprawne"<<std::endl;
            break;
    }
}
inline void cv_Mat_2_sf_Image(cv::Mat& cv_img, sf::Image& sf_img){
    //sf::Image sf_img;
//    std::make_unsigned<sf::Image> sf_img;
    cv::Mat cv_img_a;
    cv::cvtColor(cv_img, cv_img_a, cv::COLOR_BGR2RGBA);
    sf_img.create(cv_img_a.cols, cv_img_a.rows, cv_img_a.ptr());
}

inline void resize_sprite(sf::Vector2u target_size, sf::Sprite& sprite) {
    sf::Vector2u orginal_size = sprite.getTexture()->getSize();
    sf::Vector2f convert_vector = sf::Vector2f((float) target_size.x / (float) orginal_size.x,
                                               (float) target_size.y / (float) orginal_size.y);
    sprite.setScale(convert_vector);
}

inline void resize_shape(sf::Vector2u target_size, sf::Shape* shape) {
    sf::Vector2u orginal_size = shape->getTexture()->getSize();
    sf::Vector2f convert_vector = sf::Vector2f((float) target_size.x / (float) orginal_size.x,
                                               (float) target_size.y / (float) orginal_size.y);
    shape->setScale(convert_vector);
}

//inline cv::Mat load_camera_matrix(const std::string& filePath) {
//    cv::Mat result = cv::Mat::zeros(3, 3, CV_64F);
//    std::ifstream file(filePath);
//
//    if (file.is_open()) {
//        std::string line;
//        int row = 0;
//        while (std::getline(file, line) && row < 3) {
//            std::stringstream lineStream(line);
//            std::string cell;
//            int col = 0;
//            while (std::getline(lineStream, cell, ',') && col < 3) {
//                result.at<double>(row, col) = std::stof(cell);
//                auto temporary = std::stof(cell);
//                col++;
//            }
//            row++;
//        }
//        file.close();
//    } else {
//        std::cerr << "File  "<< filePath << " can't be open" << filePath << std::endl;
//    }
//    return result;
//}

inline cv::Mat load_camera_matrix(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return {};
    }

    std::vector<std::vector<double>> data;
    std::string line;
    size_t numCols = 0;

    // Read each line from the CSV file
    while (std::getline(file, line)) {
        std::stringstream lineStream(line);
        std::string cell;
        std::vector<double> row;

        while (std::getline(lineStream, cell, ',')) {
            row.push_back(std::stof(cell));  // Convert string to float
        }

        if (numCols == 0) {
            numCols = row.size();  // Set column count from the first row
        } else if (row.size() != numCols) {
            std::cerr << "Error: Inconsistent row sizes in CSV" << std::endl;
            return {};
        }

        data.push_back(row);
    }

    // Convert the 2D vector to a cv::Mat
    cv::Mat mat(data.size(), numCols, CV_64F);
    for (size_t i = 0; i < data.size(); ++i) {
        for (size_t j = 0; j < numCols; ++j) {
            mat.at<double>(i, j) = data[i][j];
        }
    }

    return mat;
}

#endif //SR_1_1_MAIN_FUNCTIONS_HPP
