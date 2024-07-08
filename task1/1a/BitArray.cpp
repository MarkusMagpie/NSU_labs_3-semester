#include "BitArray.h"
#include <algorithm> // for std::swap

// 122 урок - классы и header-файлы: https://ravesli.com/urok-122-klassy-i-zagolovochnye-fajly/

// constructors
BitArray::BitArray(int num_bits, unsigned long value) { // constructor with given number of bits
    resize(num_bits, value); // new bits are set to 0
    for (int i = 0; i < static_cast<int>(sizeof(long)) * 8 && i < num_bits; i++) {
        bits[i] = (value & (1L << i)) != 0; // != 0 is to check if the i-th bit is set
    }
}

BitArray::BitArray(const BitArray& b) { // copy constructor
    bits = b.bits;
}

void BitArray::swap(BitArray& b) {
    std::swap(bits, b.bits);
}
// assignment operator
BitArray& BitArray::operator=(const BitArray& b) {
    if (this != &b) { // 
        bits = b.bits;
    }
    return *this;
}
// resizing methods
void BitArray::resize(int num_bits, bool value) {
    bits.resize(num_bits, value);
}

void BitArray::clear() {
    bits.clear();
}

void BitArray::push_back(bool bit) {
    bits.push_back(bit);
}
// bitwise operators
BitArray& BitArray::operator&=(const BitArray& b) {
    if (bits.size() != b.bits.size()) {
        throw std::invalid_argument("BitArrays must have same size"); // exception handling
    }

    for (int i = 0; i < static_cast<int>(bits.size()); i++) {
        bits[i] = bits[i] && b.bits[i];
    }
    return *this; // return reference to the object
}

BitArray& BitArray::operator|=(const BitArray& b) {
    if (bits.size() != b.bits.size()) {
        throw std::invalid_argument("BitArrays must have same size");
    }

    for (int i = 0; i < static_cast<int>(bits.size()); i++) {
        bits[i] = bits[i] || b.bits[i];
    }
    return *this;
}

BitArray& BitArray::operator^=(const BitArray& b) {
    if (bits.size() != b.bits.size()) {
        throw std::invalid_argument("BitArrays must have same size");
    }

    for (int i = 0; i < static_cast<int>(bits.size()); i++) {
        bits[i] = bits[i] != b.bits[i];
    }
    return *this;
}
// BIT SHIFTS
// left shift: shift all bits of an object to the left by n positions, fill rightmost positions with 0
BitArray& BitArray::operator<<=(int n) {
    if (n >= static_cast<int>(bits.size())) {
        std::fill(bits.begin(), bits.end(), false); // in case of shift more than size, all bits are set to 0
    } else {
        std::rotate(bits.begin(), bits.begin() + n, bits.end());
        std::fill(bits.end() - n, bits.end(), false);
    }
    return *this;
}

BitArray& BitArray::operator>>=(int n) {
    if (n >= static_cast<int>(bits.size())) {
        std::fill(bits.begin(), bits.end(), false);
    } else {
        // std::rotate(bits.end(), bits.end() + n, bits.begin()); // not working because bits.end() + n goes out of bounds => undefined behavior
        std::rotate(bits.rbegin(), bits.rbegin() + n, bits.rend());
        std::fill(bits.begin(), bits.begin() + n, false);
    }
    return *this;
}
// copy left shift: create a copy of an object, shift it, and return the result
BitArray BitArray::operator<<(int n) const {
    BitArray res = *this;
    res <<= n;
    return res;
}

BitArray BitArray::operator>>(int n) const {
    BitArray res = *this;
    res >>= n;
    return res;
}
// set and reset methods
BitArray& BitArray::set(int n, bool val) { // bit[n] = val
    if (n < 0 || n >= static_cast<int>(bits.size())) {
        throw std::invalid_argument("Error: bit index out of range");
    }
    bits[n] = val;
    return *this;
}

BitArray& BitArray::set() { // set all bits
    for (int i = 0; i < static_cast<int>(bits.size()); i++) {
        bits[i] = true;
    }
    return *this;
}

BitArray& BitArray::reset(int n) {
    if (n < 0 || n >= static_cast<int>(bits.size())) {
        throw std::invalid_argument("Error: bit index out of range");
    }
    bits[n] = false;
    return *this;
}

BitArray& BitArray::reset() { // reset all bits
    for (int i = 0; i < static_cast<int>(bits.size()); i++) {
        bits[i] = false;
    }
    return *this;
}
// 1 - true if at leat one bit is set; 2 - true if all bits are not set
bool BitArray::any() const {
    auto BoolReturner = [](bool b) {
        return b;
    };
    return std::any_of(bits.begin(), bits.end(), BoolReturner);
    // Another option: [](bool b) { return b; } - lambda expression
    // https://www.geeksforgeeks.org/any_of-function-in-cpp-stl/
}

bool BitArray::none() const {
    return !any();
}
// bitwise inversion operator
BitArray BitArray::operator~() const { // 134: перегрузка оператора методом класса: https://ravesli.com/urok-134-peregruzka-operatorov-cherez-metody-klassa/
    BitArray current_res = *this;
    for (int i = 0; i < static_cast<int>(bits.size()); i++) {
        current_res.bits[i] = !current_res.bits[i];
    }
    return current_res;
    // Another option: std::transform(current_res.begin(), current_res.end(), current_res.begin(), [](bool b) { return !b; });
    // https://www.geeksforgeeks.org/transform-c-stl-perform-operation-elements/
}
// count number of set bits
int BitArray::count() const {
    int count = 0;
    for (int i = 0; i < static_cast<int>(bits.size()); i++) {
        if (bits[i]) {
            count++;
        }
    }
    return count;
}
// element access operator
bool BitArray::operator[](int i) const {
    if (i < 0 || i >= static_cast<int>(bits.size())) {
        throw std::invalid_argument("Error: bit index out of range");
    }
    return bits[i];
}
// size, empty methods
int BitArray::size() const {
    return bits.size();
}

bool BitArray::empty() const {
    return bits.empty();
}
// return string representation of bits
std::string BitArray::to_string() const {
    std::string res;
    for (int i = 0; i < static_cast<int>(bits.size()); i++) {
        res += bits[i] ? '1' : '0';
    }
    return res;
}
//-------------------------------------------------------
// out of class BitArray operators
bool operator==(const BitArray & a, const BitArray & b) {
    if (a.size() != b.size()) {
        return false;
    }

    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

bool operator!=(const BitArray & a, const BitArray & b) {
    return !(a == b);
}
//bitwise and, or, xor operators
BitArray operator&(const BitArray& b1, const BitArray& b2) {
    BitArray res = b1;
    res &= b2; // used operator&= from class BitArray
    return res;
}

BitArray operator|(const BitArray& b1, const BitArray& b2) {
    BitArray res = b1;
    res |= b2; // used operator|= 
    return res;
}

BitArray operator^(const BitArray& b1, const BitArray& b2) {
    BitArray res = b1;
    res ^= b2; // used operator^= 
    return res;
}