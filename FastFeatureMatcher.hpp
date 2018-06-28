#pragma once 
#include <opencv2/opencv.hpp>
#include <vector>

namespace FastFeatureMatcher {
  std::vector<std::vector<float>> getDescriptors(cv::Mat image, const std::vector<cv::KeyPoint>&);
  std::vector<cv::DMatch> match(
      cv::Mat img0, const std::vector<cv::KeyPoint>& kp0,
      cv::Mat img1, const std::vector<cv::KeyPoint>& kp1,
      float threshold = 5000
  );
};
