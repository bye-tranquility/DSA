#include "string.hpp"

String::String() = default;

String::String(size_t size, char character)
    : array_(new char[size + 1]), size_(size), cap_(size) {
  memset(array_, character, size);
  array_[size_] = '\0';
}

String::String(const char* other)
    : array_(new char[strlen(other) + 1]),
      size_(strlen(other)),
      cap_(strlen(other)) {
  memcpy(array_, other, size_ + 1);
}

String::String(const String& other) : String::String(other.array_) {}

String::String(const char* begin, const char* end)
    : array_(new char[end - begin + 2]),
      size_(end - begin + 1),
      cap_(end - begin + 1) {
  memcpy(array_, begin, size_);
  array_[size_] = '\0';
}

String& String::operator=(String other) {
  Swap(other);
  return *this;
}

void String::Swap(String& other) {
  std::swap(array_, other.array_);
  std::swap(size_, other.size_);
  std::swap(cap_, other.cap_);
}

String& String::operator+=(const String& other) {
  if (cap_ < size_ + other.size_) {
    Reserve(size_ + other.size_);
  }
  memcpy(array_ + size_, other.array_, other.size_);
  size_ += other.size_;
  array_[size_] = '\0';
  return *this;
}

String operator+(const String& str1, const String& str2) {
  if (str1.Size() > 0) {
    String result = str1;
    result += str2;
    return result;
  } else {
    return str2;
  }
}

String& String::operator*=(const unsigned int n) {
  char* prev_array = new char[size_];
  memcpy(prev_array, array_, size_);
  Resize(size_ * n);
  for (size_t i = 1; i < n; ++i) {
    memcpy(array_ + i * size_ / n, prev_array, size_ / n);
  }
  array_[size_] = '\0';
  delete[] prev_array;
  return *this;
}

String operator*(const String& string, const unsigned int n) {
  String result = string;
  result *= n;
  return result;
}

void String::Clear() {
  size_ = 0;
  if (array_ != nullptr) {
    array_[size_] = '\0';
  }
}

void String::PushBack(char character) {
  if (size_ > 0) {
    Resize(size_ + 1);
    array_[size_ - 1] = character;
  } else {
    array_ = new char[]{character, '\0'};
    size_ = 1;
    cap_ = 1;
  }
}

void String::PopBack() {
  if (size_ > 0) {
    --size_;
    array_[size_] = '\0';
  }
}

void String::Reserve(size_t new_cap) {
  if (new_cap > cap_) {
    char* new_array = new char[new_cap + 1];
    memcpy(new_array, array_, size_ + 1);
    delete[] array_;
    array_ = new_array;
    cap_ = new_cap;
  }
}

void String::Resize(size_t new_size) {
  if (new_size > cap_) {
    size_t new_cap = cap_;
    do {
      new_cap *= 2;
    } while (new_size > new_cap);
    Reserve(new_cap);
  }
  size_ = new_size;
  array_[size_] = '\0';
}

void String::Resize(size_t new_size, char character) {
  size_t prev_size = size_;
  Resize(new_size);
  memset(array_ + prev_size, character, size_ - prev_size);
  array_[size_] = '\0';
}

void String::ShrinkToFit() {
  if (cap_ > size_) {
    char* new_array = new char[size_ + 1];
    memcpy(new_array, array_, size_ + 1);
    delete[] array_;
    array_ = new_array;
    cap_ = size_;
  }
}

char& String::operator[](size_t index) { return array_[index]; }

const char& String::operator[](size_t index) const { return array_[index]; }

char& String::Front() { return array_[0]; }

const char& String::Front() const { return array_[0]; }

char& String::Back() { return array_[size_ - 1]; }

const char& String::Back() const { return array_[size_ - 1]; }

size_t String::Size() const { return size_; }

size_t String::Capacity() const { return cap_; }

const char* String::Data() const { return array_; }

bool String::Empty() const { return size_ == 0; }

int StrCompare(const char* str1, const char* str2, size_t count) {
  while (count > 0 && (*str1 != '\0') && (*str1 == *str2)) {
    ++str1;
    ++str2;
    --count;
  }
  if (count == 0) {
    return 0;
  }
  return static_cast<int>(*str1) - static_cast<int>(*str2);
}

int StrCompare(const char* str1, const char* str2) {
  return StrCompare(str1, str2, std::max(strlen(str1), strlen(str2)));
}

bool operator==(const String& str1, const String& str2) {
  if (str1.Size() != str2.Size()) {
    return false;
  }
  return StrCompare(str1.Data(), str2.Data()) == 0;
}

bool operator!=(const String& str1, const String& str2) {
  return not(str1 == str2);
}

bool operator<(const String& str1, const String& str2) {
  return StrCompare(str1.Data(), str2.Data()) < 0;
}

bool operator>(const String& str1, const String& str2) { return str2 < str1; }

bool operator>=(const String& str1, const String& str2) {
  return not(str1 < str2);
}

bool operator<=(const String& str1, const String& str2) { return str2 >= str1; }

const char* String::FindSubstring(const char* str, const char* substr) {
  size_t str_len = std::strlen(str);
  size_t substr_len = std::strlen(substr);
  if (str == nullptr || substr == nullptr || substr_len > str_len) {
    return nullptr;
  }
  for (size_t i = 0; i < str_len - substr_len + 1; ++i) {
    if (StrCompare(str + i, substr, substr_len) == 0) {
      return str + i;
    }
  }
  return nullptr;
}

std::vector<String> String::Split(const String& delim) {
  std::vector<String> result;
  const char* begin = array_;
  const char* end = array_ + size_ - 1;
  const char* delim_start = FindSubstring(begin, delim.array_);

  while (delim_start != nullptr) {
    String substr(begin, delim_start - 1);
    result.push_back(substr);
    begin = delim_start + delim.size_;
    delim_start = FindSubstring(begin, delim.array_);
  }
  String substr(begin, end);
  result.push_back(substr);
  return result;
}

String String::Join(const std::vector<String>& strings) {
  String result = "";
  if (strings.size() > 0) {
    result += strings[0];
    for (size_t i = 1; i < strings.size(); ++i) {
      result += *this;
      result += strings[i];
    }
  }
  return result;
}

String::~String() { delete[] array_; }

std::ostream& operator<<(std::ostream& out, const String& string) {
  for (size_t i = 0; i < string.Size(); ++i) {
    out << string[i];
  }
  return out;
}

std::istream& operator>>(std::istream& in, String& string) {
  string.Clear();
  char buffer;
  while (in >> buffer) {
    string.PushBack(buffer);
  }
  return in;
}