#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#define THRESH_VAL 200
#define DEBUG 1

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage " << argv[0] << " <video.mp4>" << std::endl;
    return -1;
  }

  cv::Mat frame, thresh;
  cv::VideoCapture cap(argv[1]);
  size_t fps, frame_count, width, height;

  if (!cap.isOpened()) {
    std::cerr << "Failed to play file " << argv[1] << std::endl;
    std::cerr << cv::getBuildInformation() << std::endl;
    return -2;
  }

  fps = cap.get(CV_CAP_PROP_FPS);
  frame_count = cap.get(CV_CAP_PROP_FRAME_COUNT);
  width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  if (DEBUG) {
    std::cout << "Read video file: " << argv[1] << std::endl;
    std::cout << "FPS = " << fps << std::endl;
    std::cout << "Frame Count = " << frame_count << std::endl;
    std::cout << "Frame Width = " << width << std::endl;
    std::cout << "Frame Height = " << height << std::endl;
  }

  while (cvWaitKey(10) != 27 && cap.read(frame)) {
    cv::cvtColor(frame, thresh, CV_BGR2GRAY);
    // cv::threshold(thresh, thresh, THRESH_VAL, 0xff, CV_THRESH_BINARY);
    cv::Canny(thresh, thresh, 100, 200);
    cv::imshow("window", thresh);
  }

  return 0;
}
