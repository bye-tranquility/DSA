/*
Given an array a_1, a_2, ..., a_n. Find the k-th order statistic, i.e.
k-th smallest number in the array.

How it works:
1. A new pivot is selected at each iteration.
2. The pivot is compared with the k-th order statistic.
3. Based on the comparison, the algorithm determines which part of the array
   (left or right of the pivot) contains the element.

The following code implements the median of medians idea for choosing a
derandomized pivot.

Time Complexity: O(n)
Memory Complexity: O(1)
*/

#include <algorithm>
#include <iostream>
#include <vector>

long long FindMedian(std::vector<long long>& arr, size_t left, size_t right) {
  std::sort(arr.begin() + left, arr.begin() + right + 1);
  return arr[(left + right) / 2];
}

long long DQS(std::vector<long long>& arr, size_t left, size_t right,
              size_t stat);

long long MedianOfMedians(std::vector<long long>& arr, size_t left,
                          size_t right) {
  std::vector<long long> medians;
  size_t num_elements = right - left + 1;

  if (num_elements < 5) {
    return FindMedian(arr, left, right);
  }

  for (size_t i = left; i <= right; i += 5) {
    size_t real_right = std::min(i + 4, right);
    medians.push_back(FindMedian(arr, i, real_right));
  }

  return DQS(medians, 0, medians.size() - 1, medians.size() / 2);
}

size_t Partition(std::vector<long long>& arr, size_t left, size_t right,
                 long long pivot) {
  for (size_t i = left; i < right; ++i) {
    if (arr[i] == pivot) {
      std::swap(arr[i], arr[right]);
      break;
    }
  }
  size_t i = left;
  for (size_t j = left; j < right; ++j) {
    if (arr[j] <= pivot) {
      std::swap(arr[i], arr[j]);
      ++i;
    }
  }
  std::swap(arr[i], arr[right]);
  return i;
}

long long DQS(std::vector<long long>& arr, size_t left, size_t right,
              size_t stat) {
  if (left == right) {
    return arr[left];
  }

  long long pivot = MedianOfMedians(arr, left, right);
  size_t pivot_id = Partition(arr, left, right, pivot);

  if (stat == pivot_id) {
    return arr[pivot_id];
  }
  if (stat < pivot_id) {
    return DQS(arr, left, pivot_id - 1, stat);
  }
  return DQS(arr, pivot_id + 1, right, stat);
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
  long long order_stat = DQS(arr, 0, cElemNum - 1, kth_stat);
  auto stop = std::chrono::high_resolution_clock::now();

  std::cout << "k-th order statistic: " << order_stat << '\n';
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
  std::cout << "Time taken: " << duration.count() << " ms" << '\n';  // 750-800 ms on 10 million numbers
}
