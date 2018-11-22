#include "process.hpp"
#include <opencv2/opencv.hpp>
#include <vector>

#define THRESH_VAL 50

Process::Process(const cv::Mat& src) {
  src.copyTo(this->img);
  this->kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2, 2));
}

void Process::filter_frame(cv::Mat& dst) {
  cv::cvtColor(this->img, dst, CV_BGR2GRAY);
  cv::morphologyEx(dst, dst, cv::MORPH_CLOSE, this->kernel);
  cv::morphologyEx(dst, dst, cv::MORPH_OPEN, this->kernel);
  cv::dilate(dst, dst, this->kernel, cv::Point(-1, -1), 2);
  cv::threshold(dst, dst, THRESH_VAL, 0xff, CV_THRESH_BINARY);
}

/*
int get_contours(const uint, const uint, std::vector<std::vector<cv::Point>>&){
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(fg_mask, contours, hierarchy,
                     cv::RETR_EXTERNAL, cv::CHAIN_APPROX_TC89_L1);
}
*/
