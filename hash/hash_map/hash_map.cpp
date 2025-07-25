#include "hash_map.hpp"

HashMap::HashMap(size_t cap, size_t size) : size_(size), cap_(cap) {
  map_ = InitializeHashTable(cap_);
}

HashMap::HashNode** HashMap::InitializeHashTable(size_t cap) {
  HashNode** map = new HashNode*[cap];
  for (size_t i = 0; i < cap; ++i) {
    map[i] = nullptr;
  }
  return map;
}

HashMap::~HashMap() {
  for (size_t i = 0; i < cap_; ++i) {
    HashNode* curr = map_[i];
    HashNode* prev = nullptr;

    while (curr != nullptr) {
      prev = curr;
      curr = curr->GetNext();
      delete prev;
    }
    map_[i] = nullptr;
  }
  delete[] map_;
}

bool HashMap::GetValByKey(Long key, Long& val) const {
  /*
  Returns an indicator whether the retrival proccess was successful.
  */

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

void HashMap::Insert(Long key, Long val) {
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

void HashMap::Erase(Long key) {
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

HashMap::HashNode::HashNode(Long key, Long val, HashNode* next)
    : key(key), val(val), next(next) {}

double HashMap::GetLoadFactor() const {
  return static_cast<double>(size_) / cap_;
}

void HashMap::Rehash() {
  if (GetLoadFactor() <= cMaxLoad) {
    return;
  }

  size_t new_cap = cap_ * 2;
  HashNode** new_map = InitializeHashTable(new_cap);

  HashNode* curr = nullptr;
  for (size_t i = 0; i < cap_; ++i) {
    curr = map_[i];
    while (curr != nullptr) {
      Long key = curr->GetKey();
      Long val = curr->GetVal();
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

Long HashMap::HashFunction(Long key, Long cap) {
  return (key % cap + cap) % cap;
}

Long HashMap::HashFunction(Long key) const { return HashFunction(key, cap_); }