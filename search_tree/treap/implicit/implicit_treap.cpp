#include "implicit_treap.hpp"

void ImplicitTreap::Insert(Long value, size_t pos) {
  Node* new_node = new Node(value, GetRandomPriority());
  Node *left = nullptr, *right = nullptr;
  Split(root_, pos + 1, left, right);
  Merge(left, left, new_node);
  Merge(root_, left, right);
}

Long ImplicitTreap::Sum(size_t l, size_t r) {
  Node* left = nullptr;
  Node* mid = nullptr;
  Node* right = nullptr;
  Split(root_, l, left, mid);
  Split(mid, r - l + 1, mid, right);
  Long res = GetSum(mid);
  Merge(mid, mid, right);
  Merge(root_, left, mid);
  return res;
}

void ImplicitTreap::RemoveNode(Node* node) {
  if (node == nullptr) {
    return;
  }
  RemoveNode(node->left);
  RemoveNode(node->right);
  delete node;
}

bool ImplicitTreap::Exists(Node* node, Long key) {
  if (node == nullptr) {
    return false;
  }
  if (node->key == key) {
    return true;
  }
  return Exists(key < node->key ? node->left : node->right, key);
}

Long ImplicitTreap::GetSum(Node* node) {
  return (node != nullptr) ? node->sum : 0;
}

Long ImplicitTreap::GetSize(Node* node) {
  return (node != nullptr) ? node->size : 0;
}

void ImplicitTreap::Update(Node* node) {
  if (node != nullptr) {
    node->sum = node->key + GetSum(node->left) + GetSum(node->right);
    node->size = 1 + GetSize(node->left) + GetSize(node->right);
  }
}

void ImplicitTreap::Split(Node* node, size_t pos, Node*& left, Node*& right, int add) {
  if (node == nullptr) {
    left = nullptr;
    right = nullptr;
    return;
  }
  int curr_pos = add + GetSize(node->left);
  if (pos <= curr_pos) {
    Split(node->left, pos, left, node->left, add);
    right = node;
  } else {
    Split(node->right, pos, node->right, right, curr_pos + 1);
    left = node;
  }
  Update(node);
}

void ImplicitTreap::Merge(Node*& node, Node* left, Node* right) {
  if (left == nullptr || right == nullptr) {
    node = (left != nullptr) ? left : right;
    return;
  }
  if (left->prior > right->prior) {
    Merge(left->right, left->right, right);
    node = left;
  } else {
    Merge(right->left, left, right->left);
    node = right;
  }
  Update(node);
}

void ImplicitTreap::Erase(size_t pos) {
  Node* left = nullptr;
  Node* mid = nullptr;
  Node* right = nullptr;
  Split(root_, pos, left, right);
  Split(right, 1, mid, right);
  delete mid;
  Merge(root_, left, right);
}


Long ImplicitTreap::GetRandomPriority() {
  return std::uniform_int_distribution<Long>(1, cMod)(rng_);
}