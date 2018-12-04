#include <cassert>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "process.hpp"

#define DEBUG 1
#define EPSILON 10

void draw_label(cv::Mat&, const std::string);
void draw_contours(cv::Mat&, const Contours&);

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage " << argv[0] << " <video.mp4>" << std::endl;
    return -1;
  }

  std::string text;
  uint crossed_count = 0;
  int k = 0, line_y = 0, diff = 0;
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
      if(k % 4 == 0){
    fg_img = cv::Scalar::all(0);
    bg_sub->apply(frame, fg_mask, -1);
    frame.copyTo(fg_img, fg_mask);
    Process proc(fg_img);
    proc.filter_frame(fg_img);
    Contours cntrs = proc.find_contours(fg_mask, output);
    line_y = 3 * output.rows >> 2;
    if (DEBUG)
      std::cout << "Found " << cntrs.pts.size() << " Contours" << std::endl;
    assert(cntrs.pts.size() == cntrs.centroids.size());
    cv::line(frame, cv::Point(0, line_y), cv::Point(output.cols, line_y),
             cv::Scalar(0xff, 0, 0));
    for (size_t i = 0; i < cntrs.centroids.size(); ++i) {
      if (DEBUG)
        std::cout << "Centroid y = " << cntrs.centroids[i][1]
                  << "Ref. line y = " << line_y << std::endl;
      diff = cntrs.centroids[i][1] - line_y;
      if (diff > -1 && diff <= EPSILON) {
        std::cout << "Car crossed!" << std::endl;
        ++crossed_count;
      }
    }
    if (DEBUG) std::cout << "COUNT = " << crossed_count << std::endl;
    draw_label(frame, "Count = " + std::to_string(crossed_count));
    draw_contours(output, cntrs);
    draw_contours(frame, cntrs);
    cv::imshow("Contours", output);
    cv::imshow("Traffic Detect", frame);
  }
  }
  ++k;
  return 0;
}

void draw_label(cv::Mat& im, const std::string label) {
  int fontface = cv::FONT_HERSHEY_SIMPLEX;
  double scale = 2;
  int thickness = 2;
  int baseline = 0;
  cv::Point origin(100, 100);
  cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
  cv::rectangle(im, origin + cv::Point(0, baseline),
                origin + cv::Point(text.width, -1.5 * text.height),
                CV_RGB(0xff, 0xff, 0xff), CV_FILLED);
  cv::putText(im, label, origin, fontface, scale, CV_RGB(0x0, 0x0, 0xff),
              thickness, 8);
}

void draw_contours(cv::Mat& img, const Contours& cntrs) {
  for (size_t i = 0; i < cntrs.bounding_rects.size(); ++i)
    cv::rectangle(img, cntrs.bounding_rects[i].tl(),
                  cntrs.bounding_rects[i].br(), CV_RGB(0, 0xff, 0), 2, 8, 0);
}
