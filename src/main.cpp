#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage " << argv[0] << " <video.mp4>" << std::endl;
    return -1;
  }

  cv::Mat frame;
  cv::VideoCapture cap(argv[1]);
  
  if (!cap.isOpened()) {
    std::cerr << "Failed to play file " << argv[1] << std::endl;
    return -2;
  }

  while (cvWaitKey(10) != 27 && cap.read(frame)) cv::imshow("window", frame);

  return 0;
}
