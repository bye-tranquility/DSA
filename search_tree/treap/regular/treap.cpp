#include "treap.hpp"

void Treap::Insert(Long x) {
  if (!Exists(root_, x)) {
    Node* new_node = new Node(x, GetRandomPriority());
    InsertNode(root_, new_node);
  }
}

void Treap::Erase(Long x) {
  EraseNode(root_, x);
}

Long Treap::Sum(Long l, Long r) {
  Node* left = nullptr;
  Node* mid = nullptr;
  Node* right = nullptr;
  Split(root_, l - 1, left, mid);
  Split(mid, r, mid, right);
  Long result = GetSum(mid);
  Merge(mid, mid, right);
  Merge(root_, left, mid);
  return result;
}

void Treap::RemoveNode(Node* node) {
  if (node == nullptr) {
    return;
  }
  RemoveNode(node->left);
  RemoveNode(node->right);
  delete node;
}

bool Treap::Exists(Node* node, Long key) {
  if (node == nullptr) {
    return false;
  }
  if (node->key == key) {
    return true;
  }
  return Exists(key < node->key ? node->left : node->right, key);
}

Long Treap::GetSum(Node* node) { return (node != nullptr) ? node->sum : 0; }

void Treap::UpdateSum(Node* node) {
  if (node != nullptr) {
    node->sum = node->key + GetSum(node->left) + GetSum(node->right);
  }
}

void Treap::Split(Node* node, Long key, Node*& left, Node*& right) {
  if (node == nullptr) {
    left = nullptr;
    right = nullptr;
    return;
  }
  if (node->key <= key) {
    Split(node->right, key, node->right, right);
    left = node;
  } else {
    Split(node->left, key, left, node->left);
    right = node;
  }
  UpdateSum(node);
}

void Treap::Merge(Node*& node, Node* left, Node* right) {
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
  UpdateSum(node);
}

void Treap::InsertNode(Node*& node, Node* new_node) {
  if (node == nullptr) {
    node = new_node;
  } else if (new_node->prior > node->prior) {
    Split(node, new_node->key, new_node->left, new_node->right);
    node = new_node;
  } else {
    InsertNode(new_node->key < node->key ? node->left : node->right, new_node);
  }
  UpdateSum(node);
}

void Treap::EraseNode(Node*& node, Long key) {
  if (node == nullptr) {
    return;
  }
  if (node->key == key) {
    Node* temp = node;
    Merge(node, node->left, node->right);
    delete temp;
  } else if (key < node->key) {
    EraseNode(node->left, key);
  } else {
    EraseNode(node->right, key);
  }
  UpdateSum(node);
}

Long Treap::GetRandomPriority() {
  return std::uniform_int_distribution<Long>(1, cMod)(rng_);
}