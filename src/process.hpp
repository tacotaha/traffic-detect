#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <opencv2/opencv.hpp>
#include <vector>

struct Contours {
  std::vector<std::vector<cv::Point>> pts;
  std::vector<std::array<int, 2>> centroids;
  std::vector<cv::Rect> bounding_rects;
};

class Process {
 private:
  cv::Mat img, kernel;
  static bool valid_contour(const std::vector<cv::Point>&);

 public:
  Process(const cv::Mat&);
  void filter_frame(cv::Mat&);
  Contours find_contours(const cv::Mat&, cv::Mat&);
};

#endif /* PROCESS_HPP */
