#ifndef PROCESS_HPP
#define PROCESS_HpP

#include <vector>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

typedef uint unsigned int;

class ImageProcessing{
    private:
        cv::Mat img;
    public:
        ImageProcessing(cv::Mat img) : img(img){}; 
        int filter_frame(const cv::Mat&, cv::Mat&);
        int subtract_background(const cv::Mat&, cv::Mat&);
        int get_contours(const uint, const uint, std::vector<vector<cv::Point>>&);
};

#endif /* PROCESS_HPP */
