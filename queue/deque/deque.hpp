#include <algorithm>
#include <cstddef>
#include <stdexcept>

const size_t kChunkSize = 32;
template <typename T, size_t ChunkSize = kChunkSize>
class Deque {
 public:
  Deque() = default;
  Deque(size_t count);
  Deque(size_t count, const T& value);
  Deque(const Deque& other);
  ~Deque();

  Deque& operator=(const Deque& other);
  T& operator[](size_t index);
  const T& operator[](size_t index) const;
  T& at(size_t index);
  const T& at(size_t index) const;

  size_t size() const;
  bool empty() const;

  void push_back(const T& value);
  void push_front(const T& value);
  void pop_back();
  void pop_front();

  // Iterator
  template <bool IsConst>
  class CommonIterator {
    friend class Deque;

   private:
    T** data_;
    size_t chunk_size_;
    size_t index_;

   public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = typename std::conditional_t<IsConst, const T*, T*>;
    using reference = typename std::conditional_t<IsConst, const T&, T&>;

    CommonIterator(T** data, size_t chunk_size, size_t index)
        : data_(data), chunk_size_(chunk_size), index_(index) {}

    CommonIterator(const CommonIterator& other)
        : data_(other.data_),
          chunk_size_(other.chunk_size_),
          index_(other.index_) {}

    CommonIterator& operator=(const CommonIterator& other) = default;

    reference operator*() const {
      size_t row = index_ / chunk_size_;
      size_t col = index_ % chunk_size_;
      return data_[row][col];
    }

    pointer operator->() const { return &(**this); }

    CommonIterator& operator++() {
      ++index_;
      return *this;
    }

    CommonIterator operator++(int) {
      CommonIterator tmp = *this;
      ++(*this);
      return tmp;
    }

    CommonIterator& operator--() {
      --index_;
      return *this;
    }

    CommonIterator operator--(int) {
      CommonIterator tmp = *this;
      --(*this);
      return tmp;
    }

    bool operator<(const CommonIterator& other) const {
      return index_ < other.index_;
    }

    bool operator>(const CommonIterator& other) const {
      return other.index_ < index_;
    }

    bool operator==(const CommonIterator& other) const {
      return index_ == other.index_;
    }

    bool operator>=(const CommonIterator& other) const {
      return not(*this < other);
    }

    bool operator<=(const CommonIterator& other) const {
      return other >= *this;
    }

    bool operator!=(const CommonIterator& other) const {
      return not(*this == other);
    }

    difference_type operator-(const CommonIterator& other) const {
      return index_ - other.index_;
    }

    CommonIterator operator+(difference_type num) const {
      return CommonIterator(data_, chunk_size_, index_ + num);
    }

    CommonIterator& operator+=(difference_type num) {
      index_ += num;
      return *this;
    }

    CommonIterator& operator-=(difference_type num) {
      index_ -= num;
      return *this;
    }

    CommonIterator operator-(difference_type num) const {
      return CommonIterator(data_, chunk_size_, index_ - num);
    }

    operator CommonIterator<true>() {
      return CommonIterator<true>(data_, chunk_size_, index_);
    }
  };

  using iterator = CommonIterator<false>;
  using const_iterator = CommonIterator<true>;

  iterator begin();
  const_iterator begin() const;
  const_iterator cbegin() const;
  iterator end();
  const_iterator end() const;
  const_iterator cend() const;

  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  reverse_iterator rbegin();
  reverse_iterator rend();
  const_reverse_iterator rbegin() const;
  const_reverse_iterator rend() const;
  const_reverse_iterator crbegin() const;
  const_reverse_iterator crend() const;

  void insert(iterator iter, const T& value);
  void erase(iterator iter);

 private:
  size_t cap_in_use_ = 0;  // number of chunks currently in use
  size_t total_cap_ = 0;  // total capacity of external array
  size_t size_ = 0;  // number of elements in deque
  size_t front_index_ = 0;  // first element index
  size_t back_index_ = 0;  // last element index

  T** data_ = nullptr;
  // Internal functions
  void swap(Deque& other);
  T** bigger_array();
  size_t divide_up(size_t num1, size_t num2);
  void handle_empty_push(const T& value);
  void handle_push_with_resize(bool push_back, const T& value);
  void handle_end_of_chunk_push(bool push_back, const T& value);
  void handle_regular_push(bool push_back, const T& value);
  void allocate_chunk(size_t row);
  void deallocate_chunk(size_t row);
  void destroy_in_range(size_t start_index, size_t end_index);
  void drop_arrays(size_t start_index, size_t end_index);
};

// ------------------- Constructors -------------------

template <typename T, size_t ChunkSize>
Deque<T, ChunkSize>::Deque(size_t count, const T& value)
    : cap_in_use_(divide_up(count, ChunkSize)),
      total_cap_(cap_in_use_),
      size_(count),
      front_index_(0),
      back_index_(size_),
      data_(nullptr) {
  try {
    data_ = new T*[total_cap_];
    for (size_t i = 0; i < cap_in_use_; ++i) {
      allocate_chunk(i);
    }
    size_t row;
    size_t col;
    for (size_t i = front_index_; i < back_index_; ++i) {
      row = i / ChunkSize;
      col = i % ChunkSize;
      try {
        new (&data_[row][col]) T(value);
      } catch (...) {
        destroy_in_range(front_index_, i);
        throw;
      }
    }
  } catch (...) {
    if (data_ != nullptr) {
      drop_arrays(0, cap_in_use_);
    }
    throw;
  }
}

template <typename T, size_t ChunkSize>
Deque<T, ChunkSize>::Deque(size_t count) : Deque(count, T()) {}

template <typename T, size_t ChunkSize>
Deque<T, ChunkSize>::Deque(const Deque<T, ChunkSize>& other)
    : cap_in_use_(other.cap_in_use_),
      total_cap_(other.total_cap_),
      size_(other.size_),
      front_index_(other.front_index_),
      back_index_(other.back_index_),
      data_(nullptr) {
  size_t begin_row = front_index_ / ChunkSize;
  size_t end_row = divide_up(back_index_, ChunkSize);
  try {
    data_ = new T*[other.total_cap_];
    for (size_t i = begin_row; i < end_row; ++i) {
      allocate_chunk(i);
    }
    size_t row;
    size_t col;
    for (size_t i = front_index_; i < back_index_; ++i) {
      row = i / ChunkSize;
      col = i % ChunkSize;
      try {
        new (data_[row] + col) T(other.data_[row][col]);
      } catch (...) {
        destroy_in_range(front_index_, i);
        throw;
      }
    }
  } catch (...) {
    if (data_ != nullptr) {
      drop_arrays(begin_row, end_row);
    }
    throw;
  }
}

template <typename T, size_t ChunkSize>
Deque<T, ChunkSize>::~Deque() {
  destroy_in_range(front_index_, back_index_);
  size_t first_row = front_index_ / ChunkSize;
  size_t last_row = divide_up(back_index_, ChunkSize);
  drop_arrays(first_row, last_row);
}

// ------------------- Methods --------------------

template <typename T, size_t ChunkSize>
T& Deque<T, ChunkSize>::at(size_t index) {
  if (index >= back_index_) {
    throw std::out_of_range("Index out of range");
  }
  return (*this)[index];
}

template <typename T, size_t ChunkSize>
const T& Deque<T, ChunkSize>::at(size_t index) const {
  if (index >= back_index_) {
    throw std::out_of_range("Index out of range");
  }
  return (*this)[index];
}

template <typename T, size_t ChunkSize>
size_t Deque<T, ChunkSize>::size() const {
  return size_;
}

template <typename T, size_t ChunkSize>
bool Deque<T, ChunkSize>::empty() const {
  return size_ == 0;
}

template <typename T, size_t ChunkSize>
void Deque<T, ChunkSize>::push_back(const T& value) {
  if (total_cap_ == 0) {
    handle_empty_push(value);
    return;
  }

  if (back_index_ / ChunkSize == total_cap_) {
    handle_push_with_resize(true, value);
    return;
  }

  if (back_index_ % ChunkSize == 0) {
    handle_end_of_chunk_push(true, value);
    return;
  }

  handle_regular_push(true, value);
}

template <typename T, size_t ChunkSize>
void Deque<T, ChunkSize>::push_front(const T& value) {
  if (total_cap_ == 0) {
    handle_empty_push(value);
    return;
  }

  if (front_index_ == 0) {
    handle_push_with_resize(false, value);
    return;
  }

  if (front_index_ % ChunkSize == 0) {
    handle_end_of_chunk_push(false, value);
    return;
  }

  handle_regular_push(false, value);
}

template <typename T, size_t ChunkSize>
void Deque<T, ChunkSize>::pop_back() {
  size_t curr_row = (back_index_ - 1) / ChunkSize;
  size_t curr_col = (back_index_ - 1) % ChunkSize;
  data_[curr_row][curr_col].~T();
  if (curr_col == 0) {
    deallocate_chunk(curr_row);
    --cap_in_use_;
  }
  --back_index_;
  --size_;
}

template <typename T, size_t ChunkSize>
void Deque<T, ChunkSize>::pop_front() {
  size_t curr_row = front_index_ / ChunkSize;
  size_t curr_col = front_index_ % ChunkSize;
  data_[curr_row][curr_col].~T();
  if (curr_col == ChunkSize - 1) {
    deallocate_chunk(curr_row);
    --cap_in_use_;
  }
  ++front_index_;
  --size_;
}

// ------------------ Operators -------------------

template <typename T, size_t ChunkSize>
Deque<T, ChunkSize>& Deque<T, ChunkSize>::operator=(const Deque& other) {
  if (this == &other) {
    return *this;
  }
  Deque<T, ChunkSize> copied_other(other);
  swap(copied_other);
  return *this;
}

template <typename T, size_t ChunkSize>
T& Deque<T, ChunkSize>::operator[](size_t index) {
  size_t row = (index + front_index_) / ChunkSize;
  size_t col = (index + front_index_) % ChunkSize;
  return data_[row][col];
}

template <typename T, size_t ChunkSize>
const T& Deque<T, ChunkSize>::operator[](size_t index) const {
  size_t row = (index + front_index_) / ChunkSize;
  size_t col = (index + front_index_) % ChunkSize;
  return data_[row][col];
}

// ----------------- Internal functions ------------------

template <typename T, size_t ChunkSize>
void Deque<T, ChunkSize>::swap(Deque<T, ChunkSize>& other) {
  std::swap(size_, other.size_);
  std::swap(front_index_, other.front_index_);
  std::swap(back_index_, other.back_index_);
  std::swap(cap_in_use_, other.cap_in_use_);
  std::swap(total_cap_, other.total_cap_);
  std::swap(data_, other.data_);
}

template <typename T, size_t ChunkSize>
size_t Deque<T, ChunkSize>::divide_up(size_t num1, size_t num2) {
  return (num1 + num2 - 1) / num2;
}

template <typename T, size_t ChunkSize>
T** Deque<T, ChunkSize>::bigger_array() {
  const size_t kNumberSix = 6;
  size_t new_size = cap_in_use_ + kNumberSix * divide_up(total_cap_, 2);
  T** new_data = new T*[new_size];
  size_t new_begin_row = 3 * divide_up(total_cap_, 2);
  size_t prev_begin_row = front_index_ / ChunkSize;
  for (size_t i = 0; i < cap_in_use_; ++i) {
    new_data[new_begin_row + i] = data_[prev_begin_row + i];
  }
  return new_data;
}

template <typename T, size_t ChunkSize>
void Deque<T, ChunkSize>::handle_empty_push(const T& value) {
  *this = Deque<T>(1, value);
}

template <typename T, size_t ChunkSize>
void Deque<T, ChunkSize>::handle_push_with_resize(bool push_back,
                                                  const T& value) {
  const size_t kNumberSix = 6;
  T** new_data = bigger_array();
  size_t new_row = 3 * divide_up(total_cap_, 2) + (push_back ? cap_in_use_ : -1);
  new_data[new_row] = reinterpret_cast<T*>(new char[sizeof(T) * ChunkSize]);
  try {
    new (&new_data[new_row][push_back ? 0 : (ChunkSize - 1)]) T(value);
  } catch (...) {
    delete[] reinterpret_cast<char*>(new_data[new_row]);
    delete[] new_data;
    throw;
  }
  delete[] data_;
  data_ = new_data;
  if (push_back) {
    front_index_ =
        (3 * divide_up(total_cap_, 2)) * ChunkSize + front_index_ % ChunkSize;
    back_index_ = (3 * divide_up(total_cap_, 2) + cap_in_use_) * ChunkSize + 1;
  } else {
    front_index_ = new_row * ChunkSize + ChunkSize - 1;
    back_index_ = front_index_ + size_;
  }
  total_cap_ = kNumberSix * divide_up(total_cap_, 2) + cap_in_use_;
  ++cap_in_use_;
  ++size_;
}

template <typename T, size_t ChunkSize>
void Deque<T, ChunkSize>::handle_end_of_chunk_push(bool push_back,
                                                   const T& value) {
  size_t new_row;
  if (push_back) {
    new_row = back_index_ / ChunkSize;
  } else {
    new_row = front_index_ / ChunkSize - 1;
  }
  allocate_chunk(new_row);
  try {
    new (&data_[new_row][push_back ? 0 : (ChunkSize - 1)]) T(value);
  } catch (...) {
    deallocate_chunk(new_row);
    throw;
  }
  if (push_back) {
    ++back_index_;
  } else {
    --front_index_;
  }
  ++cap_in_use_;
  ++size_;
}

template <typename T, size_t ChunkSize>
void Deque<T, ChunkSize>::handle_regular_push(bool push_back, const T& value) {
  size_t new_el_row;
  size_t new_el_col;
  if (push_back) {
    new_el_row = back_index_ / ChunkSize;
    new_el_col = back_index_ % ChunkSize;
  } else {
    new_el_row = front_index_ / ChunkSize;
    new_el_col = front_index_ % ChunkSize - 1;
  }
  try {
    new (&data_[new_el_row][new_el_col]) T(value);
  } catch (...) {
    throw;
  }
  if (push_back) {
    ++back_index_;
  } else {
    --front_index_;
  }
  ++size_;
}

template <typename T, size_t ChunkSize>
void Deque<T, ChunkSize>::allocate_chunk(size_t row) {
  data_[row] = reinterpret_cast<T*>(new char[sizeof(T) * ChunkSize]);
}

template <typename T, size_t ChunkSize>
void Deque<T, ChunkSize>::deallocate_chunk(size_t row) {
  delete[] reinterpret_cast<char*>(data_[row]);
}

template <typename T, size_t ChunkSize>
void Deque<T, ChunkSize>::destroy_in_range(size_t start_index,
                                           size_t end_index) {
  for (size_t j = start_index; j < end_index; ++j) {
    size_t row_d = j / ChunkSize;
    size_t col_d = j % ChunkSize;
    (data_[row_d] + col_d)->~T();
  }
}

template <typename T, size_t ChunkSize>
void Deque<T, ChunkSize>::drop_arrays(size_t start_index, size_t end_index) {
  for (size_t k = start_index; k < end_index; ++k) {
    deallocate_chunk(k);
  }
  delete[] data_;
}

// ------------------------ Iterator -------------------------

template <typename T, size_t ChunkSize>
typename Deque<T, ChunkSize>::iterator Deque<T, ChunkSize>::begin() {
  return iterator(data_, ChunkSize, front_index_);
}

template <typename T, size_t ChunkSize>
typename Deque<T, ChunkSize>::const_iterator Deque<T, ChunkSize>::begin()
    const {
  return const_iterator(data_, ChunkSize, front_index_);
}

template <typename T, size_t ChunkSize>
typename Deque<T, ChunkSize>::const_iterator Deque<T, ChunkSize>::cbegin()
    const {
  return const_iterator(data_, ChunkSize, front_index_);
}

template <typename T, size_t ChunkSize>
typename Deque<T, ChunkSize>::iterator Deque<T, ChunkSize>::end() {
  return iterator(data_, ChunkSize, back_index_);
}

template <typename T, size_t ChunkSize>
typename Deque<T, ChunkSize>::const_iterator Deque<T, ChunkSize>::end() const {
  return const_iterator(data_, ChunkSize, back_index_);
}

template <typename T, size_t ChunkSize>
typename Deque<T, ChunkSize>::const_iterator Deque<T, ChunkSize>::cend() const {
  return const_iterator(data_, ChunkSize, back_index_);
}

template <typename T, size_t ChunkSize>
void Deque<T, ChunkSize>::insert(iterator iter, const T& value) {
  size_t insert_pos = iter - begin();
  push_back(value);
  for (size_t i = size() - 1; i > insert_pos; --i) {
    std::swap((*this)[i], (*this)[i - 1]);
  }
}

template <typename T, size_t ChunkSize>
void Deque<T, ChunkSize>::erase(iterator iter) {
  size_t erase_pos = iter - begin();
  for (size_t i = size() - 1; i > erase_pos; --i) {
    std::swap((*this)[i], (*this)[i - 1]);
  }
  pop_back();
}

template <typename T, size_t ChunkSize>
typename Deque<T, ChunkSize>::reverse_iterator Deque<T, ChunkSize>::rbegin() {
  return reverse_iterator(end());
}

template <typename T, size_t ChunkSize>
typename Deque<T, ChunkSize>::reverse_iterator Deque<T, ChunkSize>::rend() {
  return reverse_iterator(begin());
}

template <typename T, size_t ChunkSize>
typename Deque<T, ChunkSize>::const_reverse_iterator
Deque<T, ChunkSize>::rbegin() const {
  return const_reverse_iterator(end());
}

template <typename T, size_t ChunkSize>
typename Deque<T, ChunkSize>::const_reverse_iterator Deque<T, ChunkSize>::rend()
    const {
  return const_reverse_iterator(begin());
}

template <typename T, size_t ChunkSize>
typename Deque<T, ChunkSize>::const_reverse_iterator
Deque<T, ChunkSize>::crbegin() const {
  return const_reverse_iterator(cend());
}

template <typename T, size_t ChunkSize>
typename Deque<T, ChunkSize>::const_reverse_iterator
Deque<T, ChunkSize>::crend() const {
  return const_reverse_iterator(cbegin());
}