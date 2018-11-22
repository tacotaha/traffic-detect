#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#define DEBUG 1
#define THRESH_VAL 50

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage " << argv[0] << " <video.mp4>" << std::endl;
    return -1;
  }

  size_t fps, frame_count, width, height;
  cv::Mat frame, thresh, fg_mask, fg_img, kernel;
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

  /* Train the bg subtractor on the first 500 frames */
  for (size_t i = 0; i < 500; ++i) {
    cap.read(frame);
    bg_sub->apply(frame, fg_mask, -1);
  }

  while (cvWaitKey(10) != 27 && cap.read(frame)) {
    fg_img = cv::Scalar::all(0);
    bg_sub->apply(frame, fg_mask, -1);
    frame.copyTo(fg_img, fg_mask);
    cv::cvtColor(fg_img, fg_img, CV_BGR2GRAY);
    kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2, 2));
    cv::morphologyEx(fg_img, fg_img, cv::MORPH_CLOSE, kernel);
    cv::morphologyEx(fg_img, fg_img, cv::MORPH_OPEN, kernel);
    cv::dilate(fg_img, fg_img, kernel, cv::Point(-1, -1), 2);
    cv::threshold(fg_img, fg_img, THRESH_VAL, 0xff, CV_THRESH_BINARY);
    cv::imshow("Traffic Detect", fg_img);
  }

  return 0;
}
