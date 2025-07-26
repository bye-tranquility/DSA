#include "fenwick_tree.hpp"

#include <cstdint>
#include <vector>

FenwickTree::FenwickTree(const std::vector<long long> arr)
    : fenwick_(arr.size()), size_(arr.size()) {
  for (size_t i = 0; i < size_; ++i) {
    Update(i, arr[i]);
  }
}

void FenwickTree::Update(size_t pos, long long val) {
  for (size_t i = pos; i < size_; i = (i | (i + 1))) {
    fenwick_[i] += val;
  }
}

long long FenwickTree::GetPrefixSum(long long pos) const {
  long long ans = 0;
  for (long long i = static_cast<long long>(pos); i >= 0;
       i = (i & (i + 1)) - 1) {
    ans += fenwick_[i];
  }
  return ans;
}

long long FenwickTree::GetSum(long long left, long long right) const {
  return GetPrefixSum(right) - GetPrefixSum(left - 1);
}