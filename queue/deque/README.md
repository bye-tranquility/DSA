
# Custom Deque Implementation

## Overview
A high-performance double-ended queue implementation using a chunked memory strategy with:
- O(1) amortized insertion/removal at both ends
- O(1) random access
- Full STL-compatible iterator support
- Strong exception safety guarantees
- Configurable chunk size (default: 32 elements)

## Public Interface

### Constructors & Destructor
```cpp
Deque();                          // Empty deque
Deque(size_t count);              // count default-constructed elements
Deque(size_t count, const T& value); // count copies of value
Deque(const Deque& other);        // Copy constructor
~Deque();                         // Destructor
```

### Operators
```cpp
Deque& operator=(const Deque& other); // Copy assignment
T& operator[](size_t index);       // Unchecked access
const T& operator[](size_t index) const;
```

### Capacity
```cpp
size_t size() const;
bool empty() const;
```

### Element Access
```cpp
T& at(size_t index);              // Bounds-checked access
const T& at(size_t index) const;
T& front();
const T& front() const;
T& back();
const T& back() const;
```

### Modifiers
```cpp
void push_back(const T& value);
void pop_back();
void push_front(const T& value);
void pop_front();

iterator insert(iterator pos, const T& value);
iterator erase(iterator pos);
```

### Iterators
```cpp
iterator begin();
const_iterator begin() const;
iterator end();
const_iterator end() const;

reverse_iterator rbegin();
const_reverse_iterator rbegin() const;
reverse_iterator rend();
const_reverse_iterator rend() const;
```

## Key Features

| Operation       | Complexity      | Notes                          |
|-----------------|-----------------|--------------------------------|
| push_back       | Amortized O(1)  | May allocate new chunk         |
| push_front      | Amortized O(1)  | May allocate new chunk         |
| random access   | O(1)            | Direct chunk calculation       |
| insert/erase    | O(n)            | Requires element shifting      |
| iteration      | O(1) per element| Cache-friendly chunk traversal |

## Iterator Support
- Random access iterators
- Full arithmetic operations (`+`, `-`, `+=`, `-=`)
- All comparison operators
- Reverse iterator support via `std::reverse_iterator`
- Const-correctness:
  ```cpp
  const Deque<int> d;
  auto it = d.begin(); // Returns const_iterator
  ```

## Exception Safety
### Strong Guarantee
- All modifying operations either complete successfully or leave the deque unchanged
- Exceptions during element construction/destruction are propagated

### No-throw Guarantee
- `pop_back()`, `pop_front()`
- `size()`, `empty()`
- Iterator operations (when operations on T don't throw)
## Memory Management
### Growth Strategy
1. Initial capacity: 0
2. First insertion: Allocates 1 chunk
3. Subsequent growth: Increases by ~1.5x (6 chunks minimum)

### Chunk Allocation (raw memory)
```cpp
void allocate_chunk(size_t row) {
  data_[row] = reinterpret_cast<T*>(new char[sizeof(T) * ChunkSize]);
}
```

## Usage Examples

### Basic Operations
```cpp
Deque<int> d;
d.push_back(10);
d.push_front(5);
std::cout << d[0]; // 5
d.insert(d.begin() + 1, 7); // 5,7,10
```

### Safe Access
```cpp
try {
  int val = d.at(5); // Throws if out of bounds
} catch (const std::out_of_range& e) {
  std::cerr << e.what();
}
```

### Range-based For Loop
```cpp
for (const auto& item : d) {
  std::cout << item << " ";
}
```