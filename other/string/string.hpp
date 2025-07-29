#include <cstring>
#include <iostream>
#include <vector>

class String {
 public:
  String();
  String(size_t size, char character);
  String(const char* other);
  String(const String& other);

  String& operator=(String other);
  void Swap(String& other);
  String& operator+=(const String& other);
  String& operator*=(const unsigned int n);

  void Clear();
  void PushBack(char character);
  void PopBack();
  void Resize(size_t new_size);
  void Resize(size_t new_size, char character);
  void Reserve(size_t new_cap);
  void ShrinkToFit();

  char& operator[](size_t index);
  const char& operator[](size_t index) const;
  char& Front();
  const char& Front() const;
  char& Back();
  const char& Back() const;

  size_t Size() const;
  size_t Capacity() const;
  const char* Data() const;
  bool Empty() const;

  std::vector<String> Split(const String& delim = " ");
  String Join(const std::vector<String>& strings);
  ~String();

 private:
  char* array_ = nullptr;
  size_t size_ = 0;
  size_t cap_ = 0;

  static const char* FindSubstring(const char* str, const char* substr);
  String(const char* begin, const char* end);  // Substring constructor
};

String operator+(const String& str1, const String& str2);
String operator*(const String& string, const unsigned int n);

bool operator==(const String& str1, const String& str2);
bool operator!=(const String& str1, const String& str2);
bool operator<(const String& str1, const String& str2);
bool operator>(const String& str1, const String& str2);
bool operator>=(const String& str1, const String& str2);
bool operator<=(const String& str1, const String& str2);

std::ostream& operator<<(std::ostream& out, const String& string);
std::istream& operator>>(std::istream& in, String& string);