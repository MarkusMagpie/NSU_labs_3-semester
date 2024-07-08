#pragma once

#include <iostream>
#include <vector>
#include <string>

class BitArray {
private:
  std::vector<bool> bits; // added vector to store bits in bitarray
public:
  // constructor and destructor
  BitArray() = default; // default constructor
  ~BitArray() = default; // delete memory allocated for vector bits

  // constructors
  explicit BitArray(int num_bits, unsigned long value = 0);
  BitArray(const BitArray& b);

  void swap(BitArray& b);
  // assignment operator
  BitArray& operator=(const BitArray& b);
  // resizing methods
  void resize(int num_bits, bool value = false);
  void clear();
  void push_back(bool bit);
  // bitwise operators
  BitArray& operator&=(const BitArray& b);
  BitArray& operator|=(const BitArray& b);
  BitArray& operator^=(const BitArray& b);
  // bit shifting
  BitArray& operator<<=(int n);
  BitArray& operator>>=(int n);
  BitArray operator<<(int n) const;   // constant method: https://ravesli.com/urok-123-klassy-i-const/
  BitArray operator>>(int n) const;
  // set and reset methods
  BitArray& set(int n, bool val = true);
  BitArray& set();
  BitArray& reset(int n);
  BitArray& reset();
  // 1 - true if at leat one bit is set; 2 - true if all bits are not set
  bool any() const;
  bool none() const;
  // bitwise inversion operator
  BitArray operator~() const;
  // count number of set bits
  int count() const;
  // element access operator
  bool operator[](int i) const;
  // size, empty methods
  int size() const;
  bool empty() const;
  // return string representation of bits
  std::string to_string() const;
};
// out of class BitArray operators
bool operator==(const BitArray & a, const BitArray & b);
bool operator!=(const BitArray & a, const BitArray & b);

BitArray operator&(const BitArray& b1, const BitArray& b2);
BitArray operator|(const BitArray& b1, const BitArray& b2);
BitArray operator^(const BitArray& b1, const BitArray& b2);
