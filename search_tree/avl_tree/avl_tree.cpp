#include "avl_tree.hpp"

Long AVLTree::LowerBound(Long val) {
  const Node* result = LowerBound(root_, val);
  if (result == nullptr) {
    return LLONG_MAX;
  }
  return result->val;
}

void AVLTree::UpdateHeight(Node* node) {
  size_t hleft = 0;
  size_t hright = 0;
  if (node->left != nullptr) {
    hleft = node->left->height;
  }
  if (node->right != nullptr) {
    hright = node->right->height;
  }
  node->height = std::max(hleft, hright) + 1;
}

AVLTree::Node* AVLTree::LeftRotation(Node* node) {
  Node* child = node->right;
  if (child == nullptr) {
    return node;
  }
  node->right = child->left;
  child->left = node;
  UpdateHeight(node);
  UpdateHeight(child);
  return child;
}

AVLTree::Node* AVLTree::RightRotation(Node* node) {
  Node* child = node->left;
  if (child == nullptr) {
    return node;
  }
  node->left = child->right;
  child->right = node;
  UpdateHeight(node);
  UpdateHeight(child);
  return child;
}

Long AVLTree::CalcDisbalance(const Node* node) {
  Long hleft = 0;
  Long hright = 0;
  if (node->left != nullptr) {
    hleft = node->left->height;
  }
  if (node->right != nullptr) {
    hright = node->right->height;
  }
  return hright - hleft;
}

AVLTree::Node* AVLTree::Rebalance(Node* node) {
  UpdateHeight(node);
  Long disb = CalcDisbalance(node);
  if (disb == 2) {
    if (CalcDisbalance(node->right) < 0) {
      node->right = RightRotation(node->right);
    }
    return LeftRotation(node);
  }
  if (disb == -2) {
    if (CalcDisbalance(node->left) > 0) {
      node->left = LeftRotation(node->left);
    }
    return RightRotation(node);
  }
  return node;
}

AVLTree::Node* AVLTree::Insert(Node* node, Long val) {
  if (node == nullptr) {
    return new Node(val);
  }
  if (val == node->val) {
    return node;
  }
  if (val < node->val) {
    node->left = Insert(node->left, val);
  } else {
    node->right = Insert(node->right, val);
  }
  return Rebalance(node);
}

const AVLTree::Node* AVLTree::LowerBound(const Node* node, Long val) const {
  if (node == nullptr) {
    return nullptr;
  }
  if (node->val >= val) {
    const Node* child_res = LowerBound(node->left, val);
    if (child_res != nullptr) {
      return child_res;
    }
    return node;
  }
  return LowerBound(node->right, val);
}

void AVLTree::RemoveNode(Node* node) {
  if (node == nullptr) {
    return;
  }
  RemoveNode(node->left);
  RemoveNode(node->right);
  delete node;
}