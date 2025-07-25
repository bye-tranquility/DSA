#include "heap.hpp"

void Heap::InsertKey(long long elem) {
  if (query_to_index_map_.size() <= inserted_id_) {
    query_to_index_map_.resize(inserted_id_ + 1, -1);
  }
  heap_array_.emplace_back(elem, inserted_id_);
  size_t curr_elem_idx = LastElem();
  query_to_index_map_[inserted_id_] = curr_elem_idx;
  SiftUp(curr_elem_idx);
  ++inserted_id_;
}

void Heap::SiftUp(size_t vertex_idx) {
  if (vertex_idx == 0) {
    return;
  }
  size_t parent_idx = Parent(vertex_idx);
  if (heap_array_[parent_idx] > heap_array_[vertex_idx]) {
    std::swap(query_to_index_map_[heap_array_[parent_idx].second],
              query_to_index_map_[heap_array_[vertex_idx].second]);
    std::swap(heap_array_[parent_idx], heap_array_[vertex_idx]);
    SiftUp(parent_idx);
  }
}

void Heap::SiftDown(size_t curr_idx) {
  size_t min_child_idx = curr_idx;
  size_t left_child_idx = LeftChild(curr_idx);
  size_t right_child_idx = RightChild(curr_idx);

  if (left_child_idx < heap_array_.size() &&
      heap_array_[left_child_idx] < heap_array_[min_child_idx]) {
    min_child_idx = left_child_idx;
  }

  if (right_child_idx < heap_array_.size() &&
      heap_array_[right_child_idx] < heap_array_[min_child_idx]) {
    min_child_idx = right_child_idx;
  }

  if (min_child_idx != curr_idx) {
    std::swap(query_to_index_map_[heap_array_[curr_idx].second],
              query_to_index_map_[heap_array_[min_child_idx].second]);
    std::swap(heap_array_[curr_idx], heap_array_[min_child_idx]);
    SiftDown(min_child_idx);
  }
}

void Heap::ExtractMin() {
  query_to_index_map_[heap_array_[LastElem()].second] = 0;
  query_to_index_map_[heap_array_[0].second] = -1;
  std::swap(heap_array_[0], heap_array_[LastElem()]);
  heap_array_.pop_back();
  if (!heap_array_.empty()) {
    SiftDown(0);
  }
}

void Heap::DecreaseKey(size_t inserted_id_, long long val) {
  if (inserted_id_ < query_to_index_map_.size() &&
      query_to_index_map_[inserted_id_] != static_cast<size_t>(-1)) {
    size_t idx = query_to_index_map_[inserted_id_];
    heap_array_[idx].first -= val;
    SiftUp(idx);
  }
}

long long Heap::GetMin() const {
  return heap_array_[0].first;
}
