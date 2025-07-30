# DSA
A repository containing implementations of various data structures and algorithms from different CS courses.
| Name | Category | Note
|------|------|------|
|[Deque](/queue/deque)| Queue | Chunked (block-allocated) contiguous storage, guaranteeing O(1) random access and strong exception safety.
|[MinQueue](/queue/min_queue/min_queue.hpp)| Queue | Based on two stacks
|[HashTable](/hash/hash_map/hash_map.hpp)| Hash | With separate chaining collision handling and templates support |
|[BloomFilter](/hash/bloom_filter/bloom_filter.h) | Hash | Memory-efficient (using bit representation for elements tracking)
|[MinHeap](/heap/heap.hpp)| Heap | |
|[AVL Tree](/search_tree/avl_tree/avl_tree.hpp) | Search Tree |
|[Splay Tree](/search_tree/splay_tree/splay_tree.hpp) | Search Tree | With k-th order statistic support|
|[Treap](/search_tree/treap/regular/treap.hpp) | Search Tree | Set-like data structure with Sum(l, r): $\sum\limits_{x \in [l, r]} x$ support
|[Implicit Treap](/search_tree/treap/implicit/treap.hpp) | Search Tree | Array-like data structure with Sum(l, r): $\sum\limits_{i \in [l, r]} a_i$ support
|[Segment Tree](/rmq_rsq/segment_tree/segment_tree.hpp)| RMQ/RSQ | |
|[Fenwick Tree (Binary-Indexed Tree)](/rmq_rsq/fenwick_tree/fenwick_tree.hpp)| RMQ/RSQ | |
|[Sparse Table](/rmq_rsq/sparse_table/sparse_table.hpp)| RMQ/RSQ | + second statistic support |
|[Derandomized Quick Select](/sortings/dqs.cpp) | Sortings | Via median of medians
|[Quick Select](/sortings/quick_select.cpp)| Sortings | |
|[Least Significant Digit (Radix) Sort](/sortings/lsd_sort.cpp)| Sortings | |
|[Merge Sort](/sortings/merge_sort.cpp)| Sortings | |
|[String](/other/string)| Other | Custom std::string implementation with some handy python methods
|[Longest Increasing Subsequence](/other/assembly/) | Other | ARM assembly and C implementation


