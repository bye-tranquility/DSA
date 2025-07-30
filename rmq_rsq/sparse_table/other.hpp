/*
How it works:
Sparse Table is a two-dimensional data structure st[i][j] defined as follows:
st[i][j] = min(a[i], a[i+1], …, a[i + 2^j - 1]), where j ranges from 0 to logn.
In other words, this table stores the minimum for all segments whose lengths are powers of two.

The minimums are recalculated through a recurrence relation. Additionally, this sparse table implementation supports second mins 

Time Complexity: O(nlogn) build + O(logn) for each query
Memory Complexity: O(nlogn)
*/

#include <algorithm>
#include <climits>
#include <cmath>
#include <vector>

class SparseTable {
 public:
  SparseTable(const std::vector<long long>& arr);
  long long QueryMin(long long l, long long r) const;
  long long QuerySecondMin(long long l, long long r) const;

 private:
  size_t num_count_;
  std::vector<std::vector<long long>> st_min_;
  std::vector<std::vector<long long>> st_second_min_;
  std::vector<int> log_table_;

  void BuildSparseTable(const std::vector<long long>& arr);
  void PrecomputeLogTable();
};


SparseTable::SparseTable(const std::vector<long long>& arr)
    : num_count_(arr.size()) {
  PrecomputeLogTable();
  BuildSparseTable(arr);
}

void SparseTable::PrecomputeLogTable() {
  log_table_.resize(num_count_ + 1);
  for (int i = 2; i <= num_count_; ++i) {
    log_table_[i] = log_table_[i / 2] + 1;
  }
}

void SparseTable::BuildSparseTable(const std::vector<long long>& arr) {
  int k = log_table_[num_count_] + 1;
  st_min_.resize(num_count_, std::vector<long long>(k));
  st_second_min_.resize(num_count_, std::vector<long long>(k));

  for (int i = 0; i < num_count_; ++i) {
    st_min_[i][0] = arr[i];
    st_second_min_[i][0] = LLONG_MAX;
  }

  for (int j = 1; j <= k; ++j) {
    for (int i = 0; i + (1 << j) <= num_count_; ++i) {
      int next = i + (1 << (j - 1));

      st_min_[i][j] = std::min(st_min_[i][j - 1], st_min_[next][j - 1]);

      long long candidates[4] = {st_min_[i][j - 1], st_second_min_[i][j - 1],
                                 st_min_[next][j - 1],
                                 st_second_min_[next][j - 1]};

      long long min_val = st_min_[i][j];
      long long second_min = LLONG_MAX;

      for (long long candidate : candidates) {
        if (candidate > min_val && candidate < second_min) {
          second_min = candidate;
        } else if (candidate == min_val) {
          int count = 0;
          if (st_min_[i][j - 1] == min_val) {
            count++;
          }
          if (st_min_[next][j - 1] == min_val) {
            count++;
          }
          if (st_second_min_[i][j - 1] == min_val) {
            count++;
          }
          if (st_second_min_[next][j - 1] == min_val) {
            count++;
          }

          if (count > 1) {
            second_min = min_val;
          }
        }
      }

      if (second_min == LLONG_MAX) {
        st_second_min_[i][j] = min_val;
      } else {
        st_second_min_[i][j] = second_min;
      }
    }
  }
}

long long SparseTable::QueryMin(long long l, long long r) const {
  int j = log_table_[r - l + 1];
  return std::min(st_min_[l][j], st_min_[r - (1 << j) + 1][j]);
}

long long SparseTable::QuerySecondMin(long long l, long long r) const {
  int j = log_table_[r - l + 1];
  int a = l;
  int b = r - (1 << j) + 1;

  if (st_min_[a][j] == st_min_[b][j]) {
    long long candidate1 = st_second_min_[a][j];
    long long candidate2 = st_second_min_[b][j];
    long long min_val = st_min_[a][j];

    if (candidate1 == min_val || candidate2 == min_val) {
      return min_val;
    }
    return std::min(candidate1, candidate2);
  }
  if (st_min_[a][j] < st_min_[b][j]) {
    return std::min(st_min_[b][j], st_second_min_[a][j]);
  }
  return std::min(st_min_[a][j], st_second_min_[b][j]);
}

