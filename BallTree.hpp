#pragma once

#include <vector>
#include <queue>
#include <utility>

struct BallTreeNode;

class BallTree {
private:
  int leafCapacity;
  std::vector<std::vector<float>> points;
  BallTreeNode* root;

  BallTreeNode* build(int *idx, int n);
  void nearestNeighbor(BallTreeNode* node, const std::vector<float>& point, std::priority_queue<std::pair<float, int>>& pq, int k);

public:
  BallTree(const std::vector<std::vector<float>>& _points, int leafCapacity = 16);
  ~BallTree();
  void nearestNeighbor(const std::vector<float>& point, std::priority_queue<std::pair<float, int>>& pq, int k = 1);
};
