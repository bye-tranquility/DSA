/*
How it works:
1. The array is sorted by processing fixed-size bit chunks from least to most
significant bits.
2. Each chunk is sorted using stable Counting Sort.
After all chunks are processed, the array is fully sorted.

Time Complexity: O(n * (bits / chunk_len))
Memory Complexity: O(n + 2^chunk_len)
*/

#include <cstdint>
#include <iostream>
#include <vector>

void LSDSort(std::vector<uint64_t>& nums, size_t bit_pos, size_t chunk_len) {
  const size_t cBase = 1 << chunk_len;
  const size_t cSize = nums.size();

  std::vector<size_t> cnt(cBase, 0);
  for (size_t i = 0; i < cSize; ++i) {
    size_t digit = (nums[i] >> bit_pos) & ((1 << chunk_len) - 1);
    ++cnt[digit];
  }
  for (size_t i = 1; i < cBase; ++i) {
    cnt[i] += cnt[i - 1];
  }

  std::vector<uint64_t> sorted_nums(cSize);
  // overflow of i < 0 results in cycle break
  for (size_t i = cSize - 1; i < cSize; --i) {
    size_t digit = (nums[i] >> bit_pos) & ((1 << chunk_len) - 1);
    sorted_nums[--cnt[digit]] = nums[i];
  }
  nums = sorted_nums;
}

int main() {
  const size_t cElemNum = 10'000'000;
  const int cMod = 1'000'000'000;
  std::srand(std::time(nullptr));

  std::vector<uint64_t> arr(cElemNum);
  for (size_t i = 0; i < cElemNum; ++i) {
    arr[i] = std::rand() % cMod;
  }

  const size_t cBitsChunkLen = 8;  // number of bits proccessed at one pass
  const size_t cBitsNum = 64;      // bit length of a number

  std::cout << "Running speed test for array of size: " << cElemNum << '\n';
  std::cout << "Sorting array..." << "\n";

  auto start = std::chrono::high_resolution_clock::now();
  for (size_t bit_pos = 0; bit_pos < cBitsNum; bit_pos += cBitsChunkLen) {
    LSDSort(arr, bit_pos, cBitsChunkLen);
  }
  auto stop = std::chrono::high_resolution_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
  std::cout << "Time taken: " << duration.count() << " ms" << '\n';  // 1200 ms on 10 million numbers
}