#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <ctime> // time_t
#include "FastFeatureMatcher.hpp"

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

Mat imreadWithSize(const string& name, int flag, int h, int w) {
  Mat ans;
  cv::rotate(imread(name, flag), ans, ROTATE_90_CLOCKWISE);
  resize(ans, ans, Size(w, h));
  return ans;
}

string toString(int num) {
  stringstream ss;
  ss << setw(5) << setfill('0') << num;
  return ss.str();
}

int main(int argc, char** argv ) {
  srand(time(0));
  namedWindow("Matches", WINDOW_AUTOSIZE);

  int filenum = (10 * 2 + 1);              // because there are 2 frames between 2 adjecency images
  string fileid0 = toString(filenum);
  string fileid1 = toString(filenum + 2);
  string filename0 = "images/scene" + fileid0 + ".png";
  string filename1 = "images/scene" + fileid1 + ".png";
  Mat img0 = imreadWithSize(filename0, IMREAD_GRAYSCALE, 640, 480);
  Mat img1 = imreadWithSize(filename1, IMREAD_GRAYSCALE, 640, 480);

  vector<KeyPoint> kp0, kp1;

  long long begin = clock();

  Ptr<FastFeatureDetector> fast = FastFeatureDetector::create();
  fast->detect(img0, kp0);
  fast->detect(img1, kp1);
  KeyPointsFilter::retainBest(kp0, 1000);
  KeyPointsFilter::retainBest(kp1, 1000);

  vector<DMatch> matches = FastFeatureMatcher::match(img0, kp0, img1, kp1);

  // choose 50 random matches if there are a lot of them
  random_shuffle(matches.begin(), matches.end());
  matches.resize(min((int)matches.size(), 50));

  long long end = clock();
  clog << (end - begin) * 1000.0 / CLOCKS_PER_SEC << "ms" << endl;

  Mat out;
  drawMatches(img0, kp0, img1, kp1, matches, out, Scalar::all(-1), Scalar(0, 0, 255), vector<char>()); //, DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
  imshow("Matches", out);

  //imwrite("match" + fileid0 + "-" + fileid1 + ".png", out);
  waitKey(0);
  return 0;
}
