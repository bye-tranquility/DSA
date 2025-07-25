/*
How it works:
A HashTable is a data structure that stores key-value pairs, enabling efficient
retrieval, insertion, and removal of elements based on their keys. This
efficiency is achieved through a hash function, which maps each key to an index
within an array (called buckets). This allows for average-case constant time
complexity (O(1)) for these operations.

This implementation is equivalent in functionality to the standard library's
unordered_map. It uses separate chaining with linked lists, meaning each bucket
points to the head of a linked list that contains all elements whose keys
produce the same hash value. New elements are inserted at the head of the list
for fast access.

When the load factor (the ratio of stored elements to the number of buckets)
exceeds 0.75, the table is automatically resized by doubling its capacity to
maintain efficient performance.
*/

#include <cstdio>

using Long = int64_t;

class HashMap {
 public:
  static constexpr size_t cDefaultCapacity = 16;
  static constexpr double cMaxLoad = 0.75;

  HashMap(size_t cap = cDefaultCapacity, size_t size = 0);
  ~HashMap();

  bool GetValByKey(Long key, Long& val) const;
  void Insert(Long key, Long val);
  void Erase(Long key);

 private:
  struct HashNode {
    HashNode(Long key, Long val, HashNode* next);

    HashNode* GetNext() const { return next; }
    Long GetKey() const { return key; }
    Long GetVal() const { return val; }
    void SetNext(HashNode* next) { HashNode::next = next; }
    void SetVal(Long val) { HashNode::val = val; }

    Long key;
    Long val;
    HashNode* next;
  };

  size_t size_;     // Number of elements
  size_t cap_;      // Number of buckets
  HashNode** map_;  // HashMap array

  static HashNode** InitializeHashTable(size_t cap);
  double GetLoadFactor() const;
  void Rehash();

  static Long HashFunction(Long key, Long cap);
  Long HashFunction(Long key) const;
};
