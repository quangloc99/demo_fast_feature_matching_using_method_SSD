#include <opencv2/opencv.hpp>
#include <map>
#include <vector>
#include <chrono>
#include <queue>
#include "FastFeatureMatcher.hpp"
#include "BallTree.hpp"

using namespace cv;
using namespace std;

vector<vector<float>> FastFeatureMatcher::getDescriptors(Mat img, const vector<KeyPoint>& kp) {
  vector<vector<float>> ans(kp.size(), vector<float>(16));
  
  const int px[] = {-1, 0, 1, 2, 3, 3, 3, 2, 1, 0, -1, -2, -3, -3, -3, -2};
  const int py[] = {-3, -3, -3, -2, -1, 0, 1, 2, 3, 3, 3, 2, 1, 0, -1, -2};
  for (int i = kp.size(); i--; ) {
    float x = kp[i].pt.x;
    float y = kp[i].pt.y;
    for (int f = 0; f < 16; ++f) {
      int u = img.at<unsigned char>(y + py[f], x + px[f]);
      ans[i][f] = u - 127;
    }
  }
  return ans;
}

vector<DMatch> FastFeatureMatcher::match(
  Mat img0, const vector<KeyPoint>& kp0,
  Mat img1, const vector<KeyPoint>& kp1,
  float threshold /* 5000 */
) {
  vector<vector<float>> des0 = FastFeatureMatcher::getDescriptors(img0, kp0);
  vector<vector<float>> des1 = FastFeatureMatcher::getDescriptors(img1, kp1);
  BallTree stree(des0);
  vector<DMatch> ans;

  for (int i = 0; i < des1.size(); ++i) {
    priority_queue<pair<float, int>> pq;
    stree.nearestNeighbor(des1[i], pq, 5);

    // try to choose closer point (on the image) in case
    // when the point with lowest SSD lays too far.
    int best = 0;
    float dis = 0;
    float eucldis2 = 1e9;
    while (pq.size()) {
      int f = pq.top().second;
      float d = pq.top().first;
      pq.pop();
      float dx = kp1[i].pt.x - kp0[f].pt.x;
      float dy = kp1[i].pt.y - kp0[f].pt.y;
      if (eucldis2 > dx * dx + dy * dy) {
        eucldis2 = dx * dx + dy * dy;
        dis = d * d;
        best = f;
      }
    }

    if (threshold != -1 && threshold > dis) {
      ans.push_back(DMatch(best, i, dis));
    }
  }
  return ans;
}
