/*
How it works:
AVL Tree is a binary tree with two key properties:
- Search tree property: for any node all values in its left subtree are less
than the node's value and all values in its right subtree are greater than the
node's value.
- Height property: the heights of any node's left and right subtrees differ by
at most 1. The balance is maintained through tree rotations after insertions.

The last invariant is maintained by performing left and right rotations after
insertions.

It supports inserting elements and getting lower bound of an element x (the
smallest element >= x) in O(logn) time.
*/

#include <algorithm>
#include <cstdint>
#include <string>

using Long = int64_t;

class AVLTree {
 public:
  AVLTree() : root_(nullptr) {}
  ~AVLTree() { RemoveNode(root_); }

  void Insert(Long val) { root_ = Insert(root_, val); }
  Long LowerBound(Long val);

 private:
  struct Node {
    Node(Long val) : left(nullptr), right(nullptr), val(val), height(1) {}

    Node* left;
    Node* right;
    Long val;
    Long height;
  };

  Node* root_;

  static void UpdateHeight(Node* node);
  static Node* LeftRotation(Node* node);
  static Node* RightRotation(Node* node);
  static Long CalcDisbalance(const Node* node);
  static Node* Rebalance(Node* node);
  Node* Insert(Node* node, Long val);
  const Node* LowerBound(const Node* node, Long val) const;
  void RemoveNode(Node* node);
};