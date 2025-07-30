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
