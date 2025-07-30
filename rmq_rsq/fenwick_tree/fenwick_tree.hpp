/*
How it works:
A Fenwick Tree (or Binary Indexed Tree) is a data structure for
maintaining prefix sums of an array. Each index i stores partial sums covering a
specific range, determined by the least significant bit in its binary
representation.

Two operations can be performed in O(logn) time:
1) Update: Adjusts all relevant partial sums when a value changes.
2) Main query: Aggregates the right combination of partial sums to compute the
result.

Compared to segment tree, fenwick tree is more memory efficient: it uses exactly
the same space as the original array (n elements).
*/

class FenwickTree {
 public:
  FenwickTree(const std::vector<long long>& arr);

  void Update(size_t pos, long long val);

  long long GetPrefixSum(long long pos) const;

  long long GetSum(long long left, long long right) const;

 private:
  std::vector<long long> fenwick_;
  size_t size_;
};