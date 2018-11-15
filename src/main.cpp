#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#define THRESH_VAL 200 

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage " << argv[0] << " <video.mp4>" << std::endl;
    return -1;
  }

  cv::Mat frame, thresh;
  cv::VideoCapture cap(argv[1]);
  
  if (!cap.isOpened()) {
    std::cerr << "Failed to play file " << argv[1] << std::endl;
    return -2;
  }

  while (cvWaitKey(10) != 27 && cap.read(frame)){
      cv::cvtColor(frame, thresh, CV_BGR2GRAY);
    //cv::threshold(thresh, thresh, THRESH_VAL, 0xff, CV_THRESH_BINARY); 
      cv::Canny(thresh, thresh, 100, 200);
      cv::imshow("window", thresh);
  }
  return 0;
}
