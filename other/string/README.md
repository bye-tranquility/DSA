# Custom String Implementation

## Overview
This is a custom C++ string class that provides functionality similar to `std::string`. Key features include:
- Dynamic memory management
- Common string operations (concatenation, repetition, comparison)
- Substring handling and splitting
- Stream I/O support
- Efficient resizing and capacity management

## Public Interface

### Constructors & Destructor
```cpp
String();                          // Default constructor
String(size_t size, char character); // Filled constructor
String(const char* other);          // C-string constructor
String(const String& other);        // Copy constructor
~String();                          // Destructor

// Assignment
String& operator=(String other);

// Concatenation
String& operator+=(const String& other);
String operator+(const String& other);

// Repetition
String& operator*=(unsigned int n);
String operator*(unsigned int n);

// Comparison
bool operator==(const String& other) const;
bool operator!=(const String& other) const;
bool operator<(const String& other) const;
bool operator>(const String& other) const;
bool operator>=(const String& other) const;
bool operator<=(const String& other) const;

// Element Access
char& operator[](size_t index);
const char& operator[](size_t index) const;
char& Front();
const char& Front() const;
char& Back();
const char& Back() const;
const char* Data() const;

// Capacity
size_t Size() const;
size_t Capacity() const;
bool Empty() const;
void Clear();
void Reserve(size_t new_cap);
void Resize(size_t new_size);
void Resize(size_t new_size, char character);
void ShrinkToFit();

// Modifiers
void PushBack(char character);
void PopBack();
void Swap(String& other);

// String operations
std::vector<String> Split(const String& delim = " ");  // split string by delimiter
String Join(const std::vector<String>& strings);  // join vector of strings

// I/O Operations
friend std::ostream& operator<<(std::ostream& out, const String& string);
friend std::istream& operator>>(std::istream& in, String& string);
```


## Key Features
1. **Efficient Memory Management**:  
   - Automatic resizing with `O(logn)` allocations
   - Explicit capacity control (`Reserve()`, `ShrinkToFit()`)
   
2. **String Operations**:  
   - Concatenation (`operator+`, `operator+=`)
   - Repetition (`operator*`, `operator*=` for "abc" * 3 → "abcabcabc")
   - Splitting and joining

3. **STL Compatibility**:
   - Works with standard streams (`<<`, `>>`)

## Usage Examples

### Basic Usage
```cpp
String s1("Hello");
String s2(5, 'X'); // Creates "XXXXX"
s1 += " World!";   // s1 becomes "Hello World!"

String s3 = s1 + s2; // Concatenation
String s4 = s2 * 3;  // Repetition: "XXXXX" * 3 = "XXXXXXXXXXXXXXX"
```

### String Manipulation
```cpp
String text("apple,banana,cherry");
std::vector<String> fruits = text.Split(",");

String joined = " - ".Join(fruits);
// Result: "apple - banana - cherry"
```

### Comparison
```cpp
String a("apple");
String b("banana");
if (a < b) {
    // Lexicographical comparison works
}
```

### Resizing
```cpp
String str("Hi");
str.Resize(5, '!'); // Becomes "Hi!!!"
str.PopBack();      // Becomes "Hi!!"
```