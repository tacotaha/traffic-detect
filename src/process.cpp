#include <algorithm>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include "process.hpp"

#define THRESH_VAL 50
#define MIN_RECT_HEIGHT 35
#define MIN_RECT_WIDTH 35

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

bool Process::valid_contour(const std::vector<cv::Point>& contour) {
  cv::Rect rect;
  std::vector<cv::Point> contour_pts;
  cv::approxPolyDP(cv::Mat(contour), contour_pts, 3, true);
  rect = cv::boundingRect(cv::Mat(contour_pts));
  return !(rect.width > MIN_RECT_WIDTH && rect.height > MIN_RECT_HEIGHT);
}

Contours Process::find_contours(const cv::Mat& fg_mask, cv::Mat& dst) {
  cv::Rect rect;
  std::vector<cv::Vec4i> hierarchy;
  std::vector<cv::Point> contour_pts;
  std::vector<std::array<int, 2>> centroids;
  std::vector<std::vector<cv::Point>> contours;
  dst = cv::Mat::zeros(fg_mask.size(), CV_8UC3);
  cv::findContours(fg_mask, contours, hierarchy, cv::RETR_EXTERNAL,
                   cv::CHAIN_APPROX_TC89_L1);
  contours.erase(
      std::remove_if(contours.begin(), contours.end(), valid_contour),
      contours.end());
  for (size_t i = 0; i < contours.size(); ++i) {
    cv::approxPolyDP(cv::Mat(contours[i]), contour_pts, 3, true);
    rect = cv::boundingRect(cv::Mat(contour_pts));
    centroids.push_back(
        {rect.x + (rect.width >> 1), rect.y + (rect.height >> 1)});
    cv::drawContours(dst, contours, i, cv::Scalar(0xff, 0xff, 0xff));
  }
  return {contours, centroids};
}
