#include "splay_tree.hpp"

Long SplayTree::Size() const { return root_->size; }

void SplayTree::Insert(Long val) {
  root_ = InsertNoSplay(root_, val, nullptr);
  root_ = Find(root_, val);
}

void SplayTree::Erase(Long val) {
  if (Exists(val)) {
    root_ = EraseNode(root_, val);
  }
}

bool SplayTree::Exists(Long val) { return Find(root_, val) != nullptr; }

void SplayTree::RemoveNode(Node* node) {
  if (node == nullptr) {
    return;
  }
  RemoveNode(node->left);
  RemoveNode(node->right);
  delete node;
}

void SplayTree::LeftRotation(Node* node_x) {
  Node* node_y = node_x->right;
  if (node_y == nullptr) {
    return;
  }
  node_x->right = node_y->left;
  if (node_y->left != nullptr) {
    node_y->left->parent = node_x;
  }
  node_y->parent = node_x->parent;
  if (node_x->parent == nullptr) {
    root_ = node_y;
  } else if (node_x == node_x->parent->left) {
    node_x->parent->left = node_y;
  } else {
    node_x->parent->right = node_y;
  }
  node_y->left = node_x;
  node_x->parent = node_y;
  UpdateSize(node_x);
  UpdateSize(node_y);
}

void SplayTree::RightRotation(Node* node_x) {
  Node* node_y = node_x->left;
  if (node_y == nullptr) {
    return;
  }
  node_x->left = node_y->right;
  if (node_y->right != nullptr) {
    node_y->right->parent = node_x;
  }
  node_y->parent = node_x->parent;
  if (node_x->parent == nullptr) {
    root_ = node_y;
  } else if (node_x == node_x->parent->right) {
    node_x->parent->right = node_y;
  } else {
    node_x->parent->left = node_y;
  }
  node_y->right = node_x;
  node_x->parent = node_y;
  UpdateSize(node_x);
  UpdateSize(node_y);
}

void SplayTree::Splay(Node* node) {
  while (node->parent != nullptr) {
    Node* parent = node->parent;
    Node* grandparent = parent->parent;

    if (grandparent == nullptr) {
      // zig
      if (parent->left == node) {
        RightRotation(parent);
      } else {
        LeftRotation(parent);
      }
    } 
    else if (grandparent->left == parent && parent->left == node) {
      // zig-zig (left-left)
      RightRotation(grandparent);
      RightRotation(parent);
    } 
    else if (grandparent->right == parent && parent->right == node) {
      // zig-zig (right-right)
      LeftRotation(grandparent);
      LeftRotation(parent);
    } 
    else if (grandparent->left == parent && parent->right == node) {
      // zig-zag (left-right)
      LeftRotation(parent);
      RightRotation(grandparent);
    } 
    else {
      // zig-zag (right-left)
      RightRotation(parent);
      LeftRotation(grandparent);
    }
  }
}


SplayTree::Node* SplayTree::Find(Node* node, Long val) {
  if (node == nullptr) {
    return nullptr;
  }
  if (val == node->val) {
    Splay(node);
    return node;
  }
  if (val < node->val) {
    if (node->left != nullptr) {
      return Find(node->left, val);
    }
    Splay(node);
    return nullptr;
  }
  if (node->right != nullptr) {
    return Find(node->right, val);
  }
  Splay(node);
  return nullptr;
}

SplayTree::Node* SplayTree::EraseNode(Node* root, Long val) {
  root = Find(root, val);
  if (root->val != val) {
    return root;
  }
  Node* left = root->left;
  Node* right = root->right;
  delete root;
  if (right != nullptr) {
    right->parent = nullptr;
    Node* min_right = FindMinInSubtree(right);
    Splay(min_right);
    min_right->left = left;
    if (left != nullptr) {
      left->parent = min_right;
    }
    UpdateSize(min_right);
    return min_right;
  }
  if (left != nullptr) {
    left->parent = nullptr;
    return left;
  }
  return nullptr;
}

SplayTree::Node* SplayTree::InsertNoSplay(Node* node, Long val, Node* parent) {
  if (node == nullptr) {
    Node* new_node = new Node(val);
    new_node->parent = parent;
    return new_node;
  }
  if (val == node->val) {
    return node;
  }
  if (val < node->val) {
    node->left = InsertNoSplay(node->left, val, node);
  } else {
    node->right = InsertNoSplay(node->right, val, node);
  }
  UpdateSize(node);
  return node;
}

std::optional<Long> SplayTree::Next(Long val) {
  Node* curr = root_;
  Node* next = nullptr;
  while (curr != nullptr) {
    if (curr->val > val) {
      next = curr;
      curr = curr->left;
    } else {
      curr = curr->right;
    }
  }
  if (next != nullptr) {
    Splay(next);
    return next->val;
  }
  return std::nullopt;
}

std::optional<Long> SplayTree::Prev(Long val) {
  Node* curr = root_;
  Node* prev = nullptr;
  while (curr != nullptr) {
    if (curr->val < val) {
      prev = curr;
      curr = curr->right;
    } else {
      curr = curr->left;
    }
  }
  if (prev != nullptr) {
    Splay(prev);
    return prev->val;
  }
  return std::nullopt;
}

void SplayTree::UpdateSize(Node* node) {
  if (node == nullptr) {
    return;
  }
  node->size = 1;
  if (node->left != nullptr) {
    node->size += node->left->size;
  }
  if (node->right != nullptr) {
    node->size += node->right->size;
  }
}

std::optional<Long> SplayTree::Kth(Long k) {
  if (root_ == nullptr || k >= root_->size) {
    return std::nullopt;
  }
  Node* curr = root_;
  while (curr != nullptr) {
    Long left_size = (curr->left != nullptr) ? curr->left->size : 0;
    if (k < left_size) {
      curr = curr->left;
    } else if (k == left_size) {
      Splay(curr);
      return curr->val;
    } else {
      k -= (left_size + 1);
      curr = curr->right;
    }
  }
  return std::nullopt;
}

SplayTree::Node* SplayTree::FindMinInSubtree(Node* node) {
  while (node->left != nullptr) {
    node = node->left;
  }
  return node;
}
