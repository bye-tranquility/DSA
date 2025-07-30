/*
How it works:
Sparse Table is a two-dimensional data structure st[i][j] defined as follows:
st[i][j] = min(a[i], a[i+1], …, a[i + 2^j - 1]), where j ranges from 0 to logn.
In other words, this table stores the minimum for all segments whose lengths are
powers of two.

The minimums are recalculated through a recurrence relation. Additionally, this
sparse table implementation supports premins (second statistics).

Time Complexity: O(nlogn) build + O(1) for each query
Memory Complexity: O(nlogn)
*/

#include <algorithm>
#include <climits>
#include <cmath>
#include <vector>

class SparseTable {
 public:
  SparseTable(const std::vector<long long>& arr);
  long long PreminQuery(long long l, long long r) const;
  long long MinQuery(long long l, long long r) const;

 private:
  size_t num_count_;
  std::vector<std::vector<std::pair<long long, size_t>>>
      st_min_;  // (value, index)
  std::vector<std::vector<long long>> st_premin_;
  std::vector<size_t> log_table_;

  void PrecomputeLogTable();
  void BuildSparseTable(const std::vector<long long>& arr);
  long long Premin(long long l, long long r, long long j) const;
  long long Min(long long l, long long r, long long j) const;
};

void SparseTable::PrecomputeLogTable() {
  log_table_.resize(num_count_ + 1);
  for (size_t i = 2; i <= num_count_; ++i) {
    log_table_[i] = log_table_[i / 2] + 1;
  }
}

SparseTable::SparseTable(const std::vector<long long>& arr)
    : num_count_(arr.size()) {
  PrecomputeLogTable();
  size_t log_num_count = log_table_[num_count_];
  st_min_.resize(num_count_,
                 std::vector<std::pair<long long, size_t>>(log_num_count));
  st_premin_.resize(num_count_, std::vector<long long>(log_num_count));

  BuildSparseTable(arr);
}

long long SparseTable::MinQuery(long long l, long long r) const {
  long long segment_len = r - l + 1;
  long long j = log_table_[segment_len];
  return Min(l, r, j);
}

long long SparseTable::PreminQuery(long long l, long long r) const {
  long long segment_len = r - l + 1;
  long long j = log_table_[segment_len];
  return Premin(l, r, j);
}

void SparseTable::BuildSparseTable(const std::vector<long long>& arr) {
  long long max_elem = *std::max_element(arr.begin(), arr.end());
  for (size_t i = 0; i < num_count_; ++i) {
    st_min_[i][0] = {arr[i], i};
    st_premin_[i][0] = max_elem;
  }

  for (size_t j = 1; (1 << j) <= num_count_; ++j) {
    for (size_t i = 0; i + (1 << j) <= num_count_; ++i) {
      auto left_min = st_min_[i][j - 1];
      auto right_min = st_min_[i + (1 << (j - 1))][j - 1];

      if (left_min.first < right_min.first) {
        st_min_[i][j] = left_min;
        st_premin_[i][j] = std::min(st_premin_[i][j - 1], right_min.first);
      } else if (left_min.first > right_min.first) {
        st_min_[i][j] = right_min;
        st_premin_[i][j] =
            std::min(st_premin_[i + (1 << (j - 1))][j - 1], left_min.first);
      } else {
        st_min_[i][j] = left_min;
        st_premin_[i][j] = std::min(st_premin_[i][j - 1],
                                    st_premin_[i + (1 << (j - 1))][j - 1]);
      }
    }
  }
}

long long SparseTable::Min(long long l, long long r, long long j) const {
  auto left_min = st_min_[l][j];
  auto right_min = st_min_[r - (1 << j) + 1][j];
  return std::min(left_min.first, right_min.first);
}

long long SparseTable::Premin(long long l, long long r, long long j) const {
  auto left_min = st_min_[l][j];
  auto right_min = st_min_[r - (1 << j) + 1][j];
  long long left_premin = st_premin_[l][j];
  long long right_premin = st_premin_[r - (1 << j) + 1][j];

  if (left_min.first < right_min.first) {
    return std::min(left_premin, right_min.first);
  }
  if (left_min.first > right_min.first) {
    return std::min(right_premin, left_min.first);
  }
  // Mininmums are equal, checking indices
  if (left_min.second != right_min.second) {
    return left_min.first;  // Same minimum value for two segments
  }
  return std::min(left_premin, right_premin);
}
