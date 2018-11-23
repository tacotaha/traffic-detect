#include <cassert>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "process.hpp"

#define DEBUG 0

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage " << argv[0] << " <video.mp4>" << std::endl;
    return -1;
  }

  size_t fps, frame_count, width, height;
  cv::Mat frame, thresh, fg_img, fg_mask, output;
  cv::VideoCapture cap(argv[1]);
  cv::Ptr<cv::BackgroundSubtractor> bg_sub;

  if (!cap.isOpened()) {
    std::cerr << "Failed to play file " << argv[1] << std::endl;
    std::cerr << cv::getBuildInformation() << std::endl;
    return -2;
  }

  fps = cap.get(CV_CAP_PROP_FPS);
  frame_count = cap.get(CV_CAP_PROP_FRAME_COUNT);
  width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
  bg_sub = cv::createBackgroundSubtractorMOG2();
  fg_mask.create(frame.size(), frame.type());

  if (DEBUG) {
    std::cout << "Read video file: " << argv[1] << std::endl;
    std::cout << "FPS = " << fps << std::endl;
    std::cout << "Frame Count = " << frame_count << std::endl;
    std::cout << "Frame Width = " << width << std::endl;
    std::cout << "Frame Height = " << height << std::endl;
  }

  for (size_t i = 0; i < 475; ++i) {
    cap.read(frame);
    bg_sub->apply(frame, fg_mask, -1);
  }

  while (cvWaitKey(10) != 27 && cap.read(frame)) {
    fg_img = cv::Scalar::all(0);
    bg_sub->apply(frame, fg_mask, -1);
    frame.copyTo(fg_img, fg_mask);
    Process proc(fg_img);
    proc.filter_frame(fg_img);
    Contours cntrs = proc.find_contours(fg_mask, output);
    if (DEBUG)
      std::cout << "Found " << cntrs.pts.size() << " Contours" << std::endl;
    assert(cntrs.pts.size() == cntrs.centroids.size());
    cv::line(frame, cv::Point(0, output.rows >> 1),
             cv::Point(output.cols, output.rows >> 1), cv::Scalar(0xff, 0, 0));
    for (size_t i = 0; i < cntrs.centroids.size(); ++i) {
      if (DEBUG)
        std::cout << "Centroid y = " << cntrs.centroids[i][1]
                  << "Ref. line y = " << (output.rows >> 1) << std::endl;
      if (cntrs.centroids[i][1] == output.rows >> 1)
        std::cout << "Car crossed!" << std::endl;
    }
    cv::imshow("Contours", output);
    cv::imshow("Traffic Detect", frame);
  }
  return 0;
}
