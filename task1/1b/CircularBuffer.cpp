#include "CircularBuffer.h"

#include <iostream>
#include <algorithm> // for std::swap

// default constructor and destructor
CircularBuffer::CircularBuffer() :
    buffer(nullptr),
    head(0),
    tail(0),
    buffer_size(0),
    buffer_capacity(0) {}

CircularBuffer::~CircularBuffer() {
    delete[] buffer;
}

// copy constructor
CircularBuffer::CircularBuffer(const CircularBuffer &cb) :
    buffer(new value_type[cb.buffer_capacity]),
    head(cb.head),
    tail(cb.tail),
    buffer_size(cb.buffer_size),
    buffer_capacity(cb.buffer_capacity)
{
    std::copy(cb.buffer, cb.buffer + cb.buffer_capacity, buffer);
}

// specific constructors
CircularBuffer::CircularBuffer(int capacity) :
    buffer(new value_type[capacity]),
    head(0),
    tail(0),
    buffer_size(0),
    buffer_capacity(capacity) {}

CircularBuffer::CircularBuffer(int capacity, const value_type &elem) :
    buffer(new value_type[capacity]),
    head(0),
    tail(0),
    buffer_size(capacity),
    buffer_capacity(capacity)
{
    std::fill(buffer, buffer + capacity, elem);
}

// 2 access methods via operator[] overloading
value_type &CircularBuffer::operator[](int i) {
    if (i < 0 || i >= buffer_size) {
        throw std::invalid_argument("Error: index out of range");
    }

    if (buffer_capacity == 0) {
        throw std::logic_error("Error: buffer is empty");
    }

    return buffer[(head + i) % buffer_capacity];
}

const value_type &CircularBuffer::operator[](int i) const {
    if (i < 0 || i >= buffer_size) {
        throw std::invalid_argument("Error: index out of range");
    }

    if (buffer_capacity == 0) {
        throw std::logic_error("Error: buffer is empty");
    }

    return buffer[(head + i) % buffer_capacity];
}

// 2 access methods. Throw exception if index out of range
value_type &CircularBuffer::at(int i) {
    if (i < 0 || i >= buffer_size) {
        throw std::invalid_argument("Error: index out of range");
    }

    return buffer[(head + i) % buffer_capacity];
}

const value_type &CircularBuffer::at(int i) const {
    if (i < 0 || i >= buffer_size) {
        throw std::invalid_argument("Error: index out of range");
    }
    return buffer[(head + i) % buffer_capacity];
}

// 4 methods for getting first and last elements
value_type & CircularBuffer::front() {
    if (empty()) {
        throw std::logic_error("Error: buffer is empty");
    }
    return buffer[head];
}

value_type & CircularBuffer::back() {
    if (empty()) {
        throw std::logic_error("Error: buffer is empty");
    }
    return buffer[(tail - 1 + buffer_capacity) % buffer_capacity];
}

const value_type & CircularBuffer::front() const {
    if (empty()) {
        throw std::logic_error("Error: buffer is empty");
    }
    return buffer[head];
}

const value_type & CircularBuffer::back() const {
    if (empty()) {
        throw std::logic_error("Error: buffer is empty");
    }
    return buffer[(tail - 1 + buffer_capacity) % buffer_capacity];
}

// Linearize - rearrange the buffer so that the head(first element) is at index 0 of allocated memory
// Returns a pointer to the first element
value_type * CircularBuffer::linearize() {
    if (!is_linearized()) {
        value_type *new_buffer = new value_type[buffer_capacity];
        for (int i = 0; i < buffer_size; i++) {
            new_buffer[i] = buffer[(head + i) % buffer_capacity]; // 'operator[]' is overloaded
        }

        delete[] buffer;
        buffer = new_buffer;

        head = 0;
        tail = buffer_size % buffer_capacity;
    }
    return buffer;
}

// true, if the buffer is linearized
bool CircularBuffer::is_linearized() const {
    bool linear = (head == 0 && (tail >= head || buffer_size == 0));
    std::cout << "is_linearized: head=" << head << " tail=" << tail << " buffer_size=" << buffer_size << " linear=" << linear << std::endl;
    return linear;
}

// move the ring buffer so that element with index new_begin is at 0 index
void CircularBuffer::rotate(int new_begin) {
    if (buffer_capacity == 0) {
        throw std::logic_error("Error: buffer is empty");
    }

    if (new_begin < 0 || new_begin >= buffer_size) {
        throw std::invalid_argument("Error: index out of range");
    }

    value_type *new_buffer = new value_type[buffer_capacity];
    for (int i = 0; i < buffer_size; i++) {
        new_buffer[i] = (*this)[(i + new_begin) % buffer_size];
    }

    std::swap(buffer, new_buffer);
    delete[] new_buffer;

    head = 0;
    tail = buffer_size % buffer_capacity;
}

int CircularBuffer::size() const {
    return buffer_size;
}

bool CircularBuffer::empty() const {
    return buffer_size == 0;
}

bool CircularBuffer::full() const {
    return buffer_size == buffer_capacity;
}

int CircularBuffer::reserve() const {
    return buffer_capacity - buffer_size;
}

int CircularBuffer::capacity() const {
    return buffer_capacity;
}

void CircularBuffer::set_capacity(int new_capacity) {
    if (new_capacity < buffer_size) {
        throw std::invalid_argument("Error: new capacity is less than current size");
    }

    value_type *new_buffer = new value_type[new_capacity];
    for (int i = 0; i < buffer_size; i++) {
        new_buffer[i] = (*this)[i];
    }

    std::swap(buffer, new_buffer);
    delete[] new_buffer;
    head = 0;
    tail = buffer_size % new_capacity;
    buffer_capacity = new_capacity;
}

void CircularBuffer::resize(int new_size, const value_type & item) {
    if (new_size > buffer_capacity) {
        throw std::invalid_argument("Error: new size is greater than capacity");
    }

    // if new_size > buffer_size, fill the rest with given 'item'
    if (new_size > buffer_size) {
        for (int i = buffer_size; i < new_size; i++) {
            buffer[(tail + i) % buffer_capacity] = item;
        }
    }

    buffer_size = new_size;
    tail = buffer_size % buffer_capacity;
}

// binary operator= overloading
CircularBuffer & CircularBuffer::operator=(const CircularBuffer & cb) {
    if (this != &cb) {
        CircularBuffer temp(cb);
        swap(temp);
    }

    return *this;
}

// swap method
void CircularBuffer::swap(CircularBuffer & cb) {
    std::swap(buffer, cb.buffer);
    std::swap(head, cb.head);
    std::swap(tail, cb.tail);
    std::swap(buffer_size, cb.buffer_size);
    std::swap(buffer_capacity, cb.buffer_capacity);  
}

void CircularBuffer::push_back(const value_type & item) {
    if (full()) {
        // buffer is full => overwrite head element
        head = (head + 1) % buffer_capacity;
    } else {
        // buffer is not full => simply add an item
        buffer_size++;
    }

    buffer[tail] = item;
    tail = (tail + 1) % buffer_capacity;
}

void CircularBuffer::push_front(const value_type & item) {
    if (full()) {
        tail = (tail - 1 + buffer_capacity) % buffer_capacity;
    } else {
        buffer_size++;
    }

    // to ensure proper position of new element:
    // move the head pointer to the new position before placing the new element there
    // + buffer_capacity if the head pointer is at the end of the buffer
    head = (head - 1 + buffer_capacity) % buffer_capacity;
    buffer[head] = item;
}

void CircularBuffer::pop_back() {
    if (empty()) {
        throw std::underflow_error("Error: buffer is empty");
    }

    tail = (tail - 1 + buffer_capacity) % buffer_capacity;
    buffer_size--;
    // capacity has not changed
}

void CircularBuffer::pop_front() {
    if (empty()) {
        throw std::underflow_error("Error: buffer is empty");
    }

    head = (head + 1) % buffer_capacity;
    buffer_size--;
}

void CircularBuffer::insert(int pos, const value_type & item) {
    if (pos < 0 || pos > buffer_size) {
        throw std::invalid_argument("Error: index out of range");   // or out_of_range
    }

    if (full()) {
        throw std::overflow_error("Error: buffer is full");
    }
    
    int pos_in_buffer = (head + pos) % buffer_capacity;

    // shift is required because we need to make space for a new element while maintaining the order of existing elements
    if (pos < buffer_size / 2) {
        // shift right
        for (int i = buffer_size - 1; i >= pos; --i) {
            int from = (head + i) % buffer_capacity;
            int to = (head + i + 1) % buffer_capacity;
            buffer[to] = buffer[from];
        }
        // head = (head - 1 + buffer_capacity) % buffer_capacity;
        tail = (tail + 1) % buffer_capacity;
    } else {
        for (int i = pos; i < buffer_size; ++i) {
            int from = (head + i) % buffer_capacity;
            int to = (head + i + 1) % buffer_capacity;
            buffer[to] = buffer[from];
        }
        tail = (tail + 1) % buffer_capacity;
    }

    buffer[pos_in_buffer] = item;
    buffer_size++;

    std::cout << "finished loop, final buffer:" << std::endl;
    for (int i = 0; i < buffer_size; i++) {
        std::cout << buffer[i] << " ";
    }
    std::cout << std::endl;
}

void CircularBuffer::erase(int first, int last) {
    if (first < 0 || last > buffer_size) {
        throw std::invalid_argument("Error: index out of range");
    }

    if (first > last) {
        throw std::invalid_argument("Error: invalid range");
    }

    int cnt = last - first;
    int new_head = (head + last) % buffer_capacity;
    
    if (new_head < head) {
        // new_head is before the current head, shift left from [first; buffer_size - cnt)
        // indexes of elements in the deleted range will be shifted left
        for (int i = first; i < buffer_size - cnt; i++) {
            int from = (head + i + cnt) % buffer_capacity; // [first + cnt; last + cnt) 
            int to = (head + i) % buffer_capacity; // [first; last)
            buffer[to] = buffer[from];
        }
    } else {
        // new_head is after the current head, shift right from [buffer_size - 1; last)
        for (int i = buffer_size - 1; i >= last; i--) {
            int from = (head + i) % buffer_capacity;
            int to = (head + i + cnt) % buffer_capacity;
            buffer[to] = buffer[from];
        }
    }

    buffer_size -= cnt;
    tail = (tail + cnt) % buffer_capacity;
}

// clear buffer (erase all elements) method
void CircularBuffer::clear() {
    head = 0;
    tail = 0;
    buffer_size = 0;
    buffer_capacity = 0;
}

bool operator==(const CircularBuffer & a, const CircularBuffer & b) {
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

bool operator!=(const CircularBuffer & a, const CircularBuffer & b) {
    return !(a == b);
}