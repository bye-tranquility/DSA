/*
How it works:

*/

class FenwickTree {
 public:
  FenwickTree(const std::vector<long long> arr);

  void Update(size_t pos, long long val);

  long long GetPrefixSum(long long pos) const;

  long long GetSum(long long left, long long right) const;

 private:
  std::vector<long long> fenwick_;
  size_t size_;
};