#include "bloom_filter.h"

uint64_t CalcHash(const char* str, uint64_t modulus, uint64_t seed) {
  // Polynomial hashing
  uint64_t hash_value = 0;
  uint64_t power_of_seed = 1;

  for (uint64_t i = 0; str[i] != '\0'; ++i) {
    hash_value = (hash_value + (str[i]) * power_of_seed) % modulus;
    power_of_seed = (power_of_seed * seed) % modulus;
  }
  return hash_value;
}

void Init(struct BloomFilter* bloom_filter, uint64_t set_size,
          hash_fn_t hash_fn, uint64_t hash_fn_count) {
  // Actual set size needed is 2^6 = 64 times
  // less than the number of elements as each element
  // position requires only one bit
  bloom_filter->set = (uint64_t*)calloc(set_size >> 6, sizeof(uint64_t));
  bloom_filter->set_size = set_size;
  bloom_filter->hash_fn = hash_fn;
  bloom_filter->hash_fn_count = hash_fn_count;
}

void Destroy(struct BloomFilter* bloom_filter) {
  free(bloom_filter->set);
  bloom_filter->set = NULL;
}

void Insert(struct BloomFilter* bloom_filter, Key key) {
  const uint64_t SEED = 42;  // The answer to the ultimate question of life, the
                             // universe, and everything.
  const uint64_t SET_SIZE = bloom_filter->set_size;
  const uint64_t HASH_FN_COUNT = bloom_filter->hash_fn_count;

  for (uint64_t i = 0; i < HASH_FN_COUNT; ++i) {
    uint64_t hash = CalcHash(key, SET_SIZE, SEED + i);
    uint64_t resized_hash = hash % SET_SIZE;

    uint64_t byte_pos = resized_hash >> 6;
    uint64_t bit_pos_mask = 1ULL << (resized_hash % 64);
    bloom_filter->set[byte_pos] |= bit_pos_mask;
  }
}

bool Check(struct BloomFilter* bloom_filter, Key key) {
  const uint64_t SEED = 42;
  const uint64_t SET_SIZE = bloom_filter->set_size;
  const uint64_t HASH_FN_COUNT = bloom_filter->hash_fn_count;

  for (uint64_t i = 0; i < HASH_FN_COUNT; ++i) {
    uint64_t hash = CalcHash(key, SET_SIZE, SEED + i);
    uint64_t resized_hash = hash % SET_SIZE;

    uint64_t byte_pos = resized_hash >> 6;
    uint64_t bit_pos_mask = 1ULL << (resized_hash % 64);
    if (!(bloom_filter->set[byte_pos] & bit_pos_mask)) {
      return false;
    }
  }
  return true;
}
