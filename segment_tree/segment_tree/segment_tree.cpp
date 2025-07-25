#include "segment_tree.hpp"

SegmentTree::SegmentTree(const std::vector<long long>& arr) {
  size_ = arr.size();
  tree_.resize(4 * size_);
  Build(arr, 0, size_ - 1, 1);
}

void SegmentTree::Build(const std::vector<long long>& arr, size_t left,
                        size_t right, size_t node) {
  if (left == right) {
    tree_[node] = arr[left];
  } else {
    size_t mid = (left + right) / 2;
    Build(arr, left, mid, node * 2);
    Build(arr, mid + 1, right, node * 2 + 1);
    tree_[node] = std::max(tree_[2 * node], tree_[2 * node + 1]);
  }
}

long long SegmentTree::GetMax(size_t node, size_t left, size_t right,
                              size_t left_query, size_t right_query) {
  if (right_query < left || left_query > right) {
    return LLONG_MIN;
  }
  if (left_query <= left && right_query >= right) {
    return tree_[node];
  }
  size_t mid = (left + right) / 2;
  long long left_max = GetMax(2 * node, left, mid, left_query, right_query);
  long long right_max =
      GetMax(2 * node + 1, mid + 1, right, left_query, right_query);
  return std::max(left_max, right_max);
}

void SegmentTree::Update(size_t node, size_t left, size_t right, size_t pos,
                         long long val) {
  if (left == right) {
    tree_[node] = val;
  } else {
    size_t mid = (left + right) / 2;
    if (pos <= mid) {
      Update(2 * node, left, mid, pos, val);
    } else {
      Update(2 * node + 1, mid + 1, right, pos, val);
    }
    tree_[node] = std::max(tree_[2 * node], tree_[2 * node + 1]);
  }
}
