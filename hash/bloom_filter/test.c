#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bloom_filter.h"

const char* STRINGS_TEST_CASE1[] = {"L6VoQrqkKb", "bP8d0IEpPl", "KGDYhQZubz",
                                    "sDoG9WIqjx", "UicAckq1X0", "KRyBsp1X8M",
                                    "39KbbXY66P", "rS443XVnJm"};

const char* STRINGS_TEST_CASE2[] = {"FRZzespdRO", "Ply12LOodt", "i6NJGo2r6J",
                                    "I69eTpYgvP", "I69eTpYgvP", "aZ0KGwWtmA",
                                    "lkhP9X5F6n", "qz7Zezv5gs"};

const uint64_t SET_SIZE = 16;
const uint64_t HASH_FN_COUNT = 4;
const uint64_t STRINGS_COUNT = 8;

uint64_t GetFalsePositive(const char* strings[]) {
  struct BloomFilter bloom_filter;
  Init(&bloom_filter, SET_SIZE, CalcHash, HASH_FN_COUNT);

  uint64_t false_positive_count = 0;
  for (uint64_t i = 0; i < STRINGS_COUNT; ++i) {
    bool check_result = Check(&bloom_filter, strings[i]);
    if (check_result) {
      ++false_positive_count;
    }
    Insert(&bloom_filter, strings[i]);
  }

  Destroy(&bloom_filter);

  return false_positive_count;
}

void TestInsert(const char* strings[]) {
  struct BloomFilter bloom_filter;
  Init(&bloom_filter, SET_SIZE, CalcHash, HASH_FN_COUNT);

  bool all_inserted = true;
  for (uint64_t i = 0; i < STRINGS_COUNT; ++i) {
    Insert(&bloom_filter, strings[i]);
    all_inserted &= Check(&bloom_filter, strings[i]);
  }
  Destroy(&bloom_filter);

  assert(all_inserted && "Not all values were inserted to the set");
}

int main() {
  printf(
      "Test case 1 | Number of bloom filter false positive responses: %llu\n",  // 1
      GetFalsePositive(STRINGS_TEST_CASE1));
  printf(
      "Test case 2 | Number of bloom filter false positive responses: %llu\n",  // 2
      GetFalsePositive(STRINGS_TEST_CASE2));
  TestInsert(STRINGS_TEST_CASE1);
  TestInsert(STRINGS_TEST_CASE2);
  printf("Tests passed");
  return 0;
}