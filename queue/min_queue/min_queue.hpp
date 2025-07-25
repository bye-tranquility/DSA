/*
How it works:
MinQueue is a data structure that supports queue operations (adding elements to
the end (Enqueue) and removing from the beginning (Dequeue)) while also allowing
retrieval of the smallest element in O(1) time (Min). It uses two stacks,
`push_stack` and `pop_stack` to perform corresponding operations. When
neccessary, `Relocate` function transfers elements from `push_stack` to
`pop_stack`, maintaining the queue order and updating minimum values.
*/

#include <algorithm>
#include <cstdint>
#include <stack>
#include <utility>

class MinQueue {
 public:
  void Enqueue(long long elem);
  long long Dequeue();
  long long Front();
  size_t Size() const;
  void Clear();
  long long Min();

 private:
  std::stack<std::pair<long long, long long>> push_stack_;
  std::stack<std::pair<long long, long long>> pop_stack_;
  size_t elem_count_ = 0;
  void Relocate();
};