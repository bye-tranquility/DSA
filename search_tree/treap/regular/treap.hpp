/*
How it works:
A Treap is a data structure that combines the properties of a binary search tree (BST) and a heap.

Each node stores a pair (x, y), where:
- BST property on x ("key"): for any node (x₀, y₀), all nodes in the left subtree have x < x₀, and in the right subtree x > x₀
- Heap property on y ("priority"): for any node (x₀, y₀), all nodes in both subtrees have y < y₀

Assuming all keys and priorities are distinct, this uniquely defines the treap structure.

This implementation supports the following operations in probabilistic O(logn) time:
 * Insert: add a new value to the set (ignoring duplicates)
 * Erase: remove a value
 * Sum: return the sum of all elements in the range [l, r]
*/


#include <cstdint>
#include <random>
#include <utility>

using Long = int64_t;
const int cMod = 1'000'000'000;

class Treap {
 public:
  Treap() : root_(nullptr), rng_(std::random_device{}()) {}
  ~Treap() { RemoveNode(root_); }
  void Insert(Long x);
  void Erase(Long x);
  Long Sum(Long l, Long r);

 private:
  struct Node {
    Long key;
    Long prior;
    Node* left;
    Node* right;
    Long sum;

    Node(Long key, Long prior)
        : key(key), prior(prior), left(nullptr), right(nullptr), sum(key) {}
  };

  Node* root_;
  std::mt19937 rng_;

  void RemoveNode(Node* node);
  bool Exists(Node* node, Long key);
  static Long GetSum(Node* node);
  static void UpdateSum(Node* node);
  void Split(Node* node, Long key, Node*& left, Node*& right);
  void Merge(Node*& node, Node* left, Node* right);
  void InsertNode(Node*& node, Node* new_node);
  void EraseNode(Node*& node, Long key);
  Long GetRandomPriority();
};