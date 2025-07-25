/*
How it works:
1. The unsorted array is divided into n subarrays of one element each.
2. Subarrays are repeatedly merged to produce new sorted subarrays.
The procedure is continued until there is only one array left. That is the
sorted one.

Time Complexity: O(nlogn)
Memory Complexity: O(n)
*/

#include <algorithm>
#include <iostream>
#include <vector>

void Merge(std::vector<int>& dest, const std::vector<int>& first_arr,
           const std::vector<int>& second_arr) {
  size_t size_of_first = first_arr.size();
  size_t size_of_second = second_arr.size();
  size_t i = 0;
  size_t j = 0;
  while (i < size_of_first || j < size_of_second) {
    if (j == size_of_second ||
        (i < size_of_first && first_arr[i] <= second_arr[j])) {
      dest[i + j] = first_arr[i];
      ++i;
    } else {
      dest[i + j] = second_arr[j];
      ++j;
    }
  }
}

void MergeSort(std::vector<int>& arr) {
  size_t size = arr.size();
  if (size <= 1) {
    return;
  }
  size_t half_size = size / 2;
  std::vector<int> left_part(half_size);
  std::vector<int> right_part(size - half_size);
  for (size_t i = 0; i < half_size; ++i) {
    left_part[i] = arr[i];
  }
  for (size_t i = 0; i < size - half_size; ++i) {
    right_part[i] = arr[i + half_size];
  }
  MergeSort(left_part);
  MergeSort(right_part);
  Merge(arr, left_part, right_part);
}

int main() {
  const size_t cElemNum = 10'000'000;
  const int cMod = 1'000'000'000;
  std::srand(std::time(nullptr));

  std::vector<int> arr(cElemNum);
  for (size_t i = 0; i < cElemNum; ++i) {
    arr[i] = std::rand() % cMod;
  }

  std::cout << "Running speed test for array of size: " << cElemNum << '\n';
  std::cout << "Sorting array..." << "\n";

  auto start = std::chrono::high_resolution_clock::now();
  MergeSort(arr);
  auto stop = std::chrono::high_resolution_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
  std::cout << "Time taken: " << duration.count() << " ms" << '\n';  // 7000 ms on 10 million numbers
}