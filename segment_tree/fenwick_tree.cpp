
#include <cstdint>
#include <vector>

class FenwickTree {
 public:
  FenwickTree(size_t size) : fenwick_(size, 0), size_(size) {}

  void Update(size_t pos, long long val) {
    for (size_t i = pos; i < size_; i = (i | (i + 1))) {
      fenwick_[i] += val;
    }
  }

  long long GetSum(long long pos) const {
    long long ans = 0;
    for (long long i = static_cast<long long>(pos); i >= 0;
         i = (i & (i + 1)) - 1) {
      ans += fenwick_[i];
    }
    return ans;
  }

 private:
  std::vector<long long> fenwick_;
  size_t size_;
};