/*
How it works:
Min-heap is a binary tree that satisfies two additional constraints:
- Heap property: value of each node is less that or equal to the value of its
children
- Shape property: it is a complete binary tree — all levels are fully filled
except possibly the last, which is filled from left to right.

It supports inserting new values, extracting the smallest element and decreasing
the value of an element by its insertion order index in O(logn) time.
*/

#include <algorithm>
#include <cstdint>
#include <vector>

class Heap {
 public:
  void InsertKey(long long elem);
  void SiftUp(size_t idx);
  void SiftDown(size_t idx);
  void ExtractMin();
  void DecreaseKey(size_t query_num, long long val);
  long long GetMin() const;

 private:
  size_t inserted_id_ = 0;
  std::vector<std::pair<long long, size_t>> heap_array_;
  std::vector<size_t> query_to_index_map_;

  static size_t Parent(size_t idx) { return (idx - 1) / 2; }
  static size_t LeftChild(size_t idx) { return 2 * idx + 1; }
  static size_t RightChild(size_t idx) { return 2 * idx + 2; }
  size_t LastElem() { return heap_array_.size() - 1; }
};