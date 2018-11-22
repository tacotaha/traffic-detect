#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <vector>
#include <opencv2/opencv.hpp>

struct Contours{
    std::vector<std::vector<cv::Point>> pts;
    std::vector<std::array<int, 2>> centroids;
};

class Process {
 private:
  cv::Mat img, kernel;

 public:
  Process(const cv::Mat&);
  void filter_frame(cv::Mat&);
  Contours find_contours(const cv::Mat&, cv::Mat&);
};

#endif /* PROCESS_HPP */
