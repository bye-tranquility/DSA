/*
Given an array of numbers a_1, ..., a_n. Find the longest increasing
subsequence, i.e. the sequence of indices i1 < ... < ik such that a_i1 < ... <
a_ik.

How it works:
help_array[i] stores the length of the latest found length of the LIS starting
at index i. The algorithm iterates backwards and checks if the current element
can be extended by the following elements.

There are two versions: one in C language and one in ARM Assembly.

Time Complexity: O(n^2)
Memmory Complexity: O(n)
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

uint64_t longest_inc_subseq_in_c(int64_t* array, uint64_t* help_array,
                                 uint64_t size) {
  for (size_t i = 0; i < size; ++i) {
    help_array[i] = 1;
  }

  for (int i = size - 1; i > -1; --i) {
    for (size_t j = i + 1; j < size; ++j) {
      if (array[i] < array[j] && help_array[i] < help_array[j] + 1) {
        help_array[i] = help_array[j] + 1;
      }
    }
  }
  uint64_t max_length = 0;
  for (uint64_t i = 0; i < size; ++i) {
    if (help_array[i] > max_length) {
      max_length = help_array[i];
    }
  }

  return max_length;
}

extern uint64_t longest_inc_subseq_in_asm(int64_t* array, uint64_t* help_array,
                                          uint64_t size);

int main() {
  const uint64_t ELEM_NUM = 1000000;
  const int MOD = 10000;

  int64_t* array = (int64_t*)malloc(ELEM_NUM * sizeof(int64_t));
  uint64_t* help_array = (uint64_t*)malloc(ELEM_NUM * sizeof(uint64_t));

  if (array == NULL || help_array == NULL) {
    free(array);
    free(help_array);
    return 1;
  }

  srand((unsigned)time(NULL));

  int fd = open("random_array.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd == -1) {
    free(array);
    free(help_array);
    return 1;
  }

  for (uint64_t i = 0; i < ELEM_NUM; ++i) {
    array[i] = rand() % MOD;

    char buffer[16];
    int len = snprintf(buffer, sizeof(buffer), "%lld ", array[i]);

    ssize_t bytes_written = write(fd, buffer, len);
    if (bytes_written == -1) {
      close(fd);
      free(array);
      free(help_array);
      return 1;
    }
  }

  close(fd);

  clock_t start;
  clock_t end;

  start = clock();
  uint64_t result_asm = longest_inc_subseq_in_asm(array, help_array, ELEM_NUM);
  end = clock();
  double time_asm = (double)(end - start) / CLOCKS_PER_SEC;

  start = clock();
  uint64_t result_c = longest_inc_subseq_in_c(array, help_array, ELEM_NUM);
  end = clock();
  double time_c = (double)(end - start) / CLOCKS_PER_SEC;

  printf(
      "The length of the longest increasing subsequence is: %llu (ASM) / %llu "
      "(C) \n",
      result_asm, result_c);
  printf("Time taken by function: %.1f s (ASM) / %.1f s (C) \n", time_asm,
         time_c);  // it takes 9 seconds for the assembly code to find LIS in
                   // 100'000 numbers and 13 seconds for the C language one

  free(array);
  free(help_array);
  return 0;
}
