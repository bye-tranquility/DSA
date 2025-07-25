/*
Given an array a_1, a_2, ..., a_n. Find the k-th order statistic, i.e.
k-th smallest number in the array.

How it works:
1. A random pivot is selected at each iteration.
2. The pivot is compared with the k-th order statistic.
3. Based on the comparison, the algorithm determines which part of the array
   (left or right of the pivot) contains the element.

Time Complexity: O(n) probabilistic
Memory Complexity: O(1)
*/

#include <algorithm>
#include <iostream>
#include <vector>

size_t Partition(std::vector<long long>& arr, size_t left, size_t right,
                 size_t pivot_id) {
  long long pivot_value = arr[pivot_id];
  std::swap(arr[pivot_id], arr[right]);
  size_t i = left;
  for (size_t j = left; j < right; ++j) {
    if (arr[j] <= pivot_value) {
      std::swap(arr[i], arr[j]);
      ++i;
    }
  }
  std::swap(arr[i], arr[right]);
  return i;
}

long long QuickSelect(std::vector<long long>& arr, size_t left, size_t right,
                      size_t stat) {
  if (left == right) {
    return arr[left];
  }

  size_t pivot_id = (right + left) / 2;
  pivot_id = Partition(arr, left, right, pivot_id);

  if (stat == pivot_id) {
    return arr[pivot_id];
  }
  if (stat < pivot_id) {
    return QuickSelect(arr, left, pivot_id - 1, stat);
  }
  return QuickSelect(arr, pivot_id + 1, right, stat);
}

int main() {
  const size_t cElemNum = 10'000'000;
  const int cMod = 1'000'000'000;
  std::srand(std::time(nullptr));

  size_t kth_stat = std::rand() % cElemNum;
  std::vector<long long> arr(cElemNum);
  for (size_t i = 0; i < cElemNum; ++i) {
    arr[i] = std::rand() % cMod;
  }

  std::cout << "Running speed test for array of size: " << cElemNum << '\n';
  std::cout << "Finding " << kth_stat << "-th order statistic...\n";

  auto start = std::chrono::high_resolution_clock::now();
  long long order_stat = QuickSelect(arr, 0, cElemNum - 1, kth_stat);
  auto stop = std::chrono::high_resolution_clock::now();

  std::cout << "k-th order statistic: " << order_stat << '\n';
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
  std::cout << "Time taken: " << duration.count() << " ms" << '\n';  // 100-150 ms on 10 million numbers
}
