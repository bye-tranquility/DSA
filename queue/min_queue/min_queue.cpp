#include "min_queue.hpp"

void MinQueue::Relocate() {
  if (pop_stack_.empty() && !push_stack_.empty()) {
    while (!push_stack_.empty()) {
      auto val = push_stack_.top().first;
      push_stack_.pop();
      long long new_min;
      if (!pop_stack_.empty()) {
        new_min = std::min(val, pop_stack_.top().second);
      } else {
        new_min = val;
      }
      pop_stack_.push({val, new_min});
    }
  }
}

void MinQueue::Enqueue(long long elem) {
  long long new_min;
  if (push_stack_.empty()) {
    new_min = elem;
  } else {
    new_min = std::min(elem, push_stack_.top().second);
  }
  push_stack_.push({elem, new_min});
  ++elem_count_;
}

long long MinQueue::Dequeue() {
  Relocate();
  long long front_elem = pop_stack_.top().first;
  pop_stack_.pop();
  --elem_count_;
  return front_elem;
}

long long MinQueue::Front() {
  Relocate();
  return pop_stack_.top().first;
}

long long MinQueue::Min() {
  if (push_stack_.empty()) {
    return pop_stack_.top().second;
  }
  if (pop_stack_.empty()) {
    return push_stack_.top().second;
  }
  return std::min(push_stack_.top().second, pop_stack_.top().second);
}

size_t MinQueue::Size() const { return elem_count_; }

void MinQueue::Clear() {
  while (!push_stack_.empty()) {
    push_stack_.pop();
  }
  while (!pop_stack_.empty()) {
    pop_stack_.pop();
  }
  elem_count_ = 0;
}