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
#include <functional>

template <typename T, typename Y>
class HashMap {
 public:
  static constexpr size_t cDefaultCapacity = 16;
  static constexpr double cMaxLoad = 0.75;

  HashMap(size_t cap = cDefaultCapacity, size_t size = 0);
  ~HashMap();

  bool GetValByKey(T key, Y& val) const;
  void Insert(T key, Y val);
  void Erase(T key);

 private:
  struct HashNode {
    HashNode(T key, Y val, HashNode* next);

    HashNode* GetNext() const { return next; }
    T GetKey() const { return key; }
    Y GetVal() const { return val; }
    void SetNext(HashNode* next) { this->next = next; }
    void SetVal(Y val) { this->val = val; }

    T key;
    Y val;
    HashNode* next;
  };

  size_t size_;     // Number of elements
  size_t cap_;      // Number of buckets
  HashNode** map_;  // HashMap array

  static HashNode** InitializeHashTable(size_t cap);
  double GetLoadFactor() const;
  void Rehash();

  static size_t HashFunction(T key, size_t cap);
  size_t HashFunction(T key) const;
};

template <typename T, typename Y>
HashMap<T, Y>::HashMap(size_t cap, size_t size) : size_(size), cap_(cap) {
  map_ = InitializeHashTable(cap_);
}

template <typename T, typename Y>
typename HashMap<T, Y>::HashNode** HashMap<T, Y>::InitializeHashTable(
    size_t cap) {
  HashNode** map = new HashNode*[cap];
  for (size_t i = 0; i < cap; ++i) {
    map[i] = nullptr;
  }
  return map;
}

template <typename T, typename Y>
HashMap<T, Y>::~HashMap() {
  for (size_t i = 0; i < cap_; ++i) {
    HashNode* curr = map_[i];
    while (curr != nullptr) {
      HashNode* next = curr->GetNext();
      delete curr;
      curr = next;
    }
    map_[i] = nullptr;
  }
  delete[] map_;
}

template <typename T, typename Y>
bool HashMap<T, Y>::GetValByKey(T key, Y& val) const {
  size_t bucket_idx = HashFunction(key);
  HashNode* curr = map_[bucket_idx];

  while (curr != nullptr) {
    if (curr->GetKey() == key) {
      val = curr->GetVal();
      return true;
    }
    curr = curr->GetNext();
  }
  return false;
}

template <typename T, typename Y>
void HashMap<T, Y>::Insert(T key, Y val) {
  size_t bucket_idx = HashFunction(key);

  HashNode* curr = map_[bucket_idx];
  while (curr != nullptr) {
    if (curr->GetKey() == key) {
      curr->SetVal(val);
      return;
    }
    curr = curr->GetNext();
  }

  HashNode* new_head = new HashNode(key, val, map_[bucket_idx]);
  map_[bucket_idx] = new_head;
  ++size_;

  Rehash();
}

template <typename T, typename Y>
void HashMap<T, Y>::Erase(T key) {
  size_t bucket_idx = HashFunction(key);
  HashNode* curr = map_[bucket_idx];
  HashNode* prev = nullptr;

  while (curr != nullptr) {
    if (curr->GetKey() == key) {
      if (prev == nullptr) {
        map_[bucket_idx] = curr->GetNext();
      } else {
        prev->SetNext(curr->GetNext());
      }
      delete curr;
      --size_;
      return;
    }
    prev = curr;
    curr = curr->GetNext();
  }
}

template <typename T, typename Y>
HashMap<T, Y>::HashNode::HashNode(T key, Y val, HashNode* next)
    : key(key), val(val), next(next) {}

template <typename T, typename Y>
double HashMap<T, Y>::GetLoadFactor() const {
  return static_cast<double>(size_) / cap_;
}

template <typename T, typename Y>
void HashMap<T, Y>::Rehash() {
  if (GetLoadFactor() <= cMaxLoad) {
    return;
  }

  size_t new_cap = cap_ * 2;
  HashNode** new_map = InitializeHashTable(new_cap);

  for (size_t i = 0; i < cap_; ++i) {
    HashNode* curr = map_[i];
    while (curr != nullptr) {
      T key = curr->GetKey();
      Y val = curr->GetVal();
      size_t new_bucket_idx = HashFunction(key, new_cap);

      HashNode* new_node = new HashNode(key, val, new_map[new_bucket_idx]);
      new_map[new_bucket_idx] = new_node;

      HashNode* old_node = curr;
      curr = curr->GetNext();
      delete old_node;
    }
  }

  delete[] map_;
  map_ = new_map;
  cap_ = new_cap;
}

template <typename T, typename Y>
size_t HashMap<T, Y>::HashFunction(T key, size_t cap) {
  std::hash<T> hasher;
  return hasher(key) % cap;
}

template <typename T, typename Y>
size_t HashMap<T, Y>::HashFunction(T key) const {
  return HashFunction(key, cap_);
}
