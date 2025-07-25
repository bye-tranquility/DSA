/*
How it works:
The Segment Tree is a data structure that stores information about array intervals in a tree-like format. Each node in the tree represents a specific segment of the array (a[l...r]) and aggregates its values. When traversing the tree to answer a query, the algorithm collects information from relevant nodes, combining their results to compute the final answer. This design enables efficient range queries over the array while still supporting fast modifications.

The following implementation supports two key operations in O(logn) time:
1) Finding the maximum element in a range (a[l...r]).
2) Updating an element to a new value.

It is important to mention that memory complexity of the data structure still remains linear despite its power, not being greater than 4n in size.
*/

#include <algorithm>
#include <cstdint>
#include <vector>

class SegmentTree {
 public:
  SegmentTree(const std::vector<long long>& arr);

  void Update(size_t pos, long long val) { Update(1, 0, size_ - 1, pos, val); }

  long long GetMax(size_t left, size_t right) {
    return GetMax(1, 0, size_ - 1, left, right);
  }

 private:
  std::vector<long long> tree_;
  size_t size_;

  void Build(const std::vector<long long>& arr, size_t left, size_t right,
             size_t node);

  void Update(size_t node, size_t left, size_t right, size_t pos,
              long long val);

  long long GetMax(size_t node, size_t left, size_t right, size_t left_query,
                   size_t right_query);
};
