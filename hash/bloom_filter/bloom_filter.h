/*
How it works:
A Bloom filter is a space-efficient probabilistic data structure that is used
to test whether an element is a member of a set. False positive matches are
possible but false negatives are not. In other words, a query returns either
"possibly in set" or "definitely not in set".

An empty Bloom filter is a bit array of m bits, all set to 0. A newly added
element is fed to k hash functions, each one of them gets k single-bit positions
in the array and sets it to 1 (marks as occupied). To get whether an element is
in the set, the reverse operation is done: the same k positions are retrivied;
if any of the bits at these positions is 0, the element is definitely not in the
set, otherwise either the element is in the set, or the bits have by chance been
set to 1 during the insertion of other elements, resulting in a false positive.

Fun Fact:
The Google Chrome web browser previously used a Bloom filter to identify
malicious URLs. Any URL was first checked against a local Bloom filter, and only
if the Bloom filter returned a positive result was a full check of the URL
performed (and the user was warned, if that too returned a positive result).
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef uint64_t (*hash_fn_t)(const char*, uint64_t, uint64_t);

uint64_t CalcHash(const char* str, uint64_t modulus, uint64_t seed);

struct BloomFilter {
  uint64_t* set;
  uint64_t set_size;
  hash_fn_t hash_fn;
  uint64_t hash_fn_count;
};
typedef const char* Key;

void Init(struct BloomFilter* bloom_filter, uint64_t set_size,
          hash_fn_t hash_fn, uint64_t hash_fn_count);

void Destroy(struct BloomFilter* bloom_filter);

void Insert(struct BloomFilter* bloom_filter, Key key);

bool Check(struct BloomFilter* bloom_filter, Key key);