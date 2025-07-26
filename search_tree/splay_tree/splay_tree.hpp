/*
How it works:
Splay Tree is a binary tree with two key properties:
- Search tree property: for any node all values in its left subtree are less
than the node's value and all values in its right subtree are greater than the
node's value.Ï
- Temporal locality property: recently accessed elements are moved to the root.

All operations on a binary search tree are combined with one basic operation,
called `splay`. Splaying the tree for a certain element rearranges the tree so
that the element is placed at the root of the tree. Specific combination of tree
rotations (`zig`, `zig-zig` and `zig-zag`) is used in order to do so.

This implementation supports the following operations in amortized O(logn) time:
 * Insert: add a new value to the set (ignoring duplicates)
 * Erase: remove a value
 * Next: find the smallest value greater than the given one
 * Prev: find the greatest value smaller than the given one
 * Kth: return the k-th order statistic (in 0-indexation)
 * Exists: check whether a value is present
*/

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <optional>

using Long = int64_t;

class SplayTree {
 public:
  SplayTree() : root_(nullptr) {}
  ~SplayTree() { RemoveNode(root_); }

  void Insert(Long val);
  void Erase(Long val);
  std::optional<Long> Next(Long val);
  std::optional<Long> Prev(Long val);
  std::optional<Long> Kth(Long k);
  bool Exists(Long val);
  Long Size() const;

 private:
  struct Node {
    Node(Long val)
        : left(nullptr), right(nullptr), parent(nullptr), val(val), size(1) {}

    Node* left;
    Node* right;
    Node* parent;
    Long val;
    Long size;
  };

  Node* root_;

  void RemoveNode(Node* node);
  void LeftRotation(Node* node_x);
  void RightRotation(Node* node_x);
  void Splay(Node* node);
  Node* Find(Node* node, Long val);
  Node* EraseNode(Node* root, Long val);
  Node* InsertNoSplay(Node* node, Long val, Node* parent);

  static void UpdateSize(Node* node);
  static Node* FindMinInSubtree(Node* node);
};
