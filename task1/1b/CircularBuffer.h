#pragma once

typedef char value_type;

class CircularBuffer {
private:
    value_type *buffer;		// array of elements in the buffer
    int head;				// index of the first element
    int tail; 				// index of the last element
    int buffer_size; 		// number of elements
    int buffer_capacity; 	// max number of elements
public:
    // CONSTRUCTORS AND DESTRUCTORS-------------------------------------------------
    CircularBuffer();
	CircularBuffer(const CircularBuffer &cb);
    explicit CircularBuffer(int capacity); // explicit prohibits implicit conversion
    CircularBuffer(int capacity, const value_type &elem);
	
    ~CircularBuffer();

    // ACCESS METHODS-------------------------------------------------------------
    // index access methods (overloading)
    value_type & operator[](int i);
    const value_type & operator[](int i) const;
    
    // element access methods (overloading). Throw exception if i is out of range
    value_type & at(int i);
    const value_type & at(int i) const;

    value_type & front(); // link to the first reference
    value_type & back();  // link to the last reference
    const value_type & front() const;
    const value_type & back() const;

    // LINEARIZATION AND SIZE/CAPACITY METHODS (Buffer Management)----------------
    // linearization - move the ring buffer so that the first element 
    // is moved to the beginning of the allocated memory. 
    // returns a pointer to the first element
    value_type * linearize();

    // true, if the buffer is linearized
    bool is_linearized() const;

    // move the ring buffer so that element with index new_begin is at 0 index
    void rotate(int new_begin);

    // number of elements currently in the buffer
    int size() const;

    // true, if buffer_size == 0
    bool empty() const;

    // true, if buffer_size == buffer_capacity;
    bool full() const;

    // number of elements that can be stored in the buffer (capacity - size)
    int reserve() const;

    // number of elements that can be placed in a buffer without the need to allocate additional memory
    int capacity() const;

    // set the capacity of the buffer
    void set_capacity(int new_capacity);

    // change buffer size. In case of expansion, new elements are filled with "item"
    void resize(int new_size, const value_type & item = value_type());

    //------------------------------------------------------------------------------
    // assignment operator
    CircularBuffer &operator=(const CircularBuffer & cb);

    // swaps the content of cb buffer with the content of this buffer
    void swap(CircularBuffer & cb);

    // adds new item to the end of the buffer
    // if the buffer is full (size = capacity), the first element is overwritten
    void push_back(const value_type & item = value_type());

    // adds new item to the beginning of the buffer (also could overwrite the last buffer element)
    void push_front(const value_type & item = value_type());

    // deletes the last element of the buffer
    void pop_back();
    // deletes the first element of the buffer
    void pop_front();

    // puts item at position pos. The capacity of the buffer remains unchanged.
    void insert(int pos, const value_type & item = value_type());

    // deletes elements in the range [first, last) from the buffer
    void erase(int first, int last);
    void clear();

    friend bool operator==(const CircularBuffer & a, const CircularBuffer & b);
    friend bool operator!=(const CircularBuffer & a, const CircularBuffer & b);
};