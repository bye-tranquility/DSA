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
