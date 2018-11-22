#include <iostream>
#include <opencv2/opencv.hpp>

#include "process.hpp"

#define DEBUG 1

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage " << argv[0] << " <video.mp4>" << std::endl;
    return -1;
  }

  size_t fps, frame_count, width, height;
  cv::Mat frame, thresh, fg_img, fg_mask;
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
    cv::Mat output = cv::Mat::zeros(frame.size(), CV_8UC3);
    Contours cntrs = proc.find_contours(fg_mask, output);
    cv::imshow("Traffic Detect", output);
  }

  return 0;
}
