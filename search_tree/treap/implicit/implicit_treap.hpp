/*
How it works:
An Implicit Treap is a data structure that combines the properties of a binary
search tree (BST) and a heap, but instead of using explicit keys for ordering,
it maintains implicit indices based on the subtree sizes. When traversing the
tree, subtree sizes of all nodes to the left are summed to recalculate indices.

Each node stores a pair (x, y), where:
- BST property on x ("key"): for any node (x₀, y₀), all nodes in the left
subtree have x < x₀, and in the right subtree x > x₀
- Heap property on y ("priority"): for any node (x₀, y₀), all nodes in both
subtrees have y < y₀

Assuming all keys and priorities are distinct, this uniquely defines the treap
structure.

This implementation supports the following operations in probabilistic O(logn)
time:
 * Insert: add a new value to the array to positon `pos` (in 0-based indexation)
 * Erase: remove an element at position `pos`
 * Sum: return the sum of all elements in a range a[l...r]
*/

#include <cstdint>
#include <random>
#include <utility>

using Long = int64_t;
const int cMod = 1'000'000'000;

class ImplicitTreap {
 public:
  ImplicitTreap() : root_(nullptr), rng_(std::random_device{}()) {}
  ~ImplicitTreap() { RemoveNode(root_); }
  void Erase(size_t pos);
  void Insert(Long value, size_t pos);
  Long Sum(size_t l, size_t r);

 private:
  struct Node {
    Long key;
    Long prior;
    Node* left;
    Node* right;
    Long sum;
    size_t size;

    Node(Long key, Long prior)
        : key(key), prior(prior), left(nullptr), right(nullptr), sum(key) {}
  };

  Node* root_;
  std::mt19937 rng_;

  void RemoveNode(Node* node);
  bool Exists(Node* node, Long key);
  static Long GetSize(Node* node);
  static Long GetSum(Node* node);
  static void Update(Node* node);
  void Split(Node* node, size_t pos, Node*& left, Node*& right, int add = 0);
  void Merge(Node*& node, Node* left, Node* right);
  Long GetRandomPriority();
};