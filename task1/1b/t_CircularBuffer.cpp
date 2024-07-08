#include <gtest/gtest.h>        // Google Test header
#include "CircularBuffer.h"

// test fixture for CircularBuffer tests
class CircularBufferTest : public ::testing::Test {
protected:
    void SetUp() override {}        // called before each test
    void TearDown() override {}     // called after each test (cleanup)
    // member variables accessible in all test cases
    CircularBuffer buffer;
};

// test case for default constructor; size and empty methods
TEST_F(CircularBufferTest, ConstructorDefaultTest) {
    EXPECT_EQ(buffer.size(), 0);
    EXPECT_TRUE(buffer.empty());
}

// test case for copy constructor
TEST_F(CircularBufferTest, CopyConstructor) {
    CircularBuffer buffer1(3);
    buffer1.push_back('A');
    buffer1.push_back('B');

    CircularBuffer buffer2(buffer1);    // created a copy of buffer1 and assigned it to buffer2 using copy constructor

    for (int i = 0; i < buffer2.size(); ++i) {
        EXPECT_EQ(buffer2[i], buffer1[i]);
    }
}

// test case for constructor with capacity (add tests for other private elements of the class)
TEST_F(CircularBufferTest, ConstructorWithCapacity) {
    CircularBuffer a(5);
    
    EXPECT_EQ(a.size(), 0);
    EXPECT_EQ(a.capacity(), 5);
}

TEST_F(CircularBufferTest, ConstructorWithCapacityAndFillValue) {
    CircularBuffer a(5, 'A');       // create a buffer with capacity (and size) 5 and fill it with 'A'
    EXPECT_EQ(a.size(), 5);
    EXPECT_EQ(a.capacity(), 5);

    for (int i = 0; i < 5; ++i) {   // check that all elements are 'A'
        EXPECT_EQ(a[i], 'A');
    }
}

// Test cases for operator[] - operator[] overloading
TEST_F(CircularBufferTest, OperatorBracketOverflow) {
    buffer.set_capacity(2);

    buffer.push_back('A');
    buffer.push_back('B');

    EXPECT_EQ(buffer[0], 'A');
    EXPECT_EQ(buffer[1], 'B');
}

TEST_F(CircularBufferTest, AtValidTest) {
    buffer.set_capacity(2);

    buffer.push_back('A');
    buffer.push_back('B');

    EXPECT_EQ(buffer.at(0), 'A');
    EXPECT_EQ(buffer.at(1), 'B');
}

TEST_F(CircularBufferTest, AtInvalidTest) {
    buffer.set_capacity(1);
    buffer.push_back('A');

    EXPECT_THROW(buffer.at(1), std::invalid_argument);
}

TEST_F(CircularBufferTest, FrontBackTest) {
    buffer.set_capacity(2);

    buffer.push_back('A');
    buffer.push_back('B');

    EXPECT_EQ(buffer.front(), 'A');
    EXPECT_EQ(buffer.back(), 'B');
}

TEST_F(CircularBufferTest, LinearizeTest) {
    buffer.set_capacity(3);

    buffer.push_back('A');
    buffer.push_back('B');
    buffer.push_back('C');

    value_type *ptr = buffer.linearize();

    EXPECT_EQ(buffer.size(), 3);
    EXPECT_EQ(buffer.capacity(), 3);

    EXPECT_EQ(*ptr, 'A');
    EXPECT_EQ(*(ptr + 1), 'B');
    EXPECT_EQ(*(ptr + 2), 'C');   
}

// Buffer is linear if there is no wrapped elements (head is at start). 
// So, we make it non-linear by making head = 1!
TEST_F(CircularBufferTest, IsLinearizedTest) {
    buffer.set_capacity(3);

    buffer.push_back('A');                      // buffer state: [A, _, _]; head = 0, tail = 1
    buffer.push_back('B');                      // buffer state: [A, B, _]; head = 0, tail = 2
    buffer.push_back('C');                      // buffer state: [A, B, C]; head = 0, tail = 3 = 0

    buffer.pop_front();                         // buffer state: [_, B, C]; head = 1, tail = 0
    buffer.push_back('D');                      // buffer state: [D, B, C]; head = 1, tail = 1

    std::cout << "Buffer state before linearize:" << std::endl;
    for (int i = 0; i < buffer.size(); ++i) {
        std::cout << buffer[i] << " ";
    }
    std::cout << std::endl;

    EXPECT_FALSE(buffer.is_linearized());       // buffer should not be linearized after wrapping around

    buffer.linearize();                         // buffer state: [A, B, C]; head = 0, tail = 3
    EXPECT_TRUE(buffer.is_linearized());

    std::cout << "Buffer state after linearize:" << std::endl;
    for (int i = 0; i < buffer.size(); ++i) {
        std::cout << buffer[i] << " ";
    }
    std::cout << std::endl;
}

TEST_F(CircularBufferTest, RotateTest) {
    buffer.set_capacity(4);

    buffer.push_back('A');
    buffer.push_back('B');
    buffer.push_back('C');
    buffer.push_back('D');

    buffer.rotate(2);

    EXPECT_EQ(buffer.front(), 'C');
    EXPECT_EQ(buffer.back(), 'B');
}

TEST_F(CircularBufferTest, SizeTest) {
    buffer.set_capacity(1);
    EXPECT_EQ(buffer.size(), 0);

    buffer.push_back('A');
    EXPECT_EQ(buffer.size(), 1);

    buffer.pop_back();                         // of buffer.pop_front();  
    EXPECT_EQ(buffer.size(), 0);
}

TEST_F(CircularBufferTest, EmptyTest) {
    buffer.set_capacity(1);
    EXPECT_TRUE(buffer.empty());

    buffer.push_back('A');
    EXPECT_FALSE(buffer.empty());
}

TEST_F(CircularBufferTest, FullTest) {
    // upon adding elements both size and capacity increase; upon deleting only size decreases
    // https://ravesli.com/urok-106-emkost-vektora-std-vector-v-kachestve-steka/
    CircularBuffer buffer(2);                   // create a buffer with capacity 2 and size 0
    EXPECT_FALSE(buffer.full());

    buffer.push_back('A');
    buffer.push_back('B');                      // buffer state: [A, B]; size = 2; capacity = 2
    EXPECT_TRUE(buffer.full());

    buffer.pop_back();                          // buffer state: [A, _]; size = 1; capacity = 2 
    EXPECT_FALSE(buffer.full());
}

TEST_F(CircularBufferTest, ReserveTest) {
    EXPECT_EQ(buffer.reserve(), 0);

    CircularBuffer buffer(5);
    EXPECT_EQ(buffer.capacity(), 5);
    EXPECT_EQ(buffer.size(), 0);
    EXPECT_EQ(buffer.reserve(), 5);
}

TEST_F(CircularBufferTest, CapacityTest) {
    EXPECT_EQ(buffer.capacity(), 0);

    buffer.set_capacity(1);
    buffer.push_back('A');
    EXPECT_EQ(buffer.capacity(), 1);
}

TEST_F(CircularBufferTest, SetCapacityTest) {
    buffer.set_capacity(3);
    EXPECT_EQ(buffer.capacity(), 3);

    buffer.set_capacity(2);
    EXPECT_EQ(buffer.capacity(), 2);
}

TEST_F(CircularBufferTest, ResizeTest) {
    buffer.set_capacity(3);         // size cannot exceed capacity
    buffer.resize(3, 'A');
    EXPECT_EQ(buffer.size(), 3);

    for (int i = 0; i < 3; i++) {
        EXPECT_EQ(buffer[i], 'A');
    }

    buffer.resize(2);
    EXPECT_EQ(buffer.size(), 2);
}

// Test cases for operator= method
TEST_F(CircularBufferTest, AssignOperatorTest) {
    CircularBuffer buffer1(3);
    buffer1.push_back('A');
    buffer1.push_back('B');

    CircularBuffer buffer2 = buffer1;

    EXPECT_EQ(buffer2.front(), 'A');
    EXPECT_EQ(buffer2.back(), 'B');
    EXPECT_EQ(buffer2.size(), 2);
}

TEST_F(CircularBufferTest, SwapTest) {
    // i will create 2 separate buffers and swap them
    CircularBuffer buffer1(3);
    buffer1.push_back('A');
    buffer1.push_back('B');

    CircularBuffer buffer2(2);
    buffer2.push_back('C');

    buffer1.swap(buffer2);

    EXPECT_EQ(buffer1.size(), 1);       // size changed due to usage of push_back() method
    EXPECT_EQ(buffer1.front(), 'C');

    EXPECT_EQ(buffer2.size(), 2);
    EXPECT_EQ(buffer2.front(), 'A');
    EXPECT_EQ(buffer2.back(), 'B');
}

TEST_F(CircularBufferTest, PushBackTest) {
    buffer.set_capacity(2);
    
    buffer.push_back('A');
    buffer.push_back('B');

    EXPECT_EQ(buffer.size(), 2);
    EXPECT_TRUE(buffer.full());

    EXPECT_EQ(buffer.front(), 'A');
    EXPECT_EQ(buffer.back(), 'B');
}

TEST_F(CircularBufferTest, PushFrontTest) {
    buffer.set_capacity(2);

    buffer.push_front('A');
    buffer.push_front('B');

    EXPECT_EQ(buffer.size(), 2);
    EXPECT_TRUE(buffer.full());

    EXPECT_EQ(buffer.front(), 'B');
    EXPECT_EQ(buffer.back(), 'A');
}

TEST_F(CircularBufferTest, PopBackTest) {
    buffer.set_capacity(2);

    buffer.push_back('A');
    buffer.push_back('B');

    buffer.pop_back();

    EXPECT_EQ(buffer.size(), 1);
    EXPECT_EQ(buffer.capacity(), 2);
    EXPECT_FALSE(buffer.full());

    EXPECT_EQ(buffer.front(), 'A');
    EXPECT_EQ(buffer.back(), 'A');
}

TEST_F(CircularBufferTest, PopFrontTest) {
    buffer.set_capacity(2);

    buffer.push_back('A');
    buffer.push_back('B');

    buffer.pop_front();

    EXPECT_EQ(buffer.size(), 1);
    EXPECT_EQ(buffer.capacity(), 2);
    EXPECT_FALSE(buffer.full());

    EXPECT_EQ(buffer.front(), 'B');
    EXPECT_EQ(buffer.back(), 'B');
}

TEST_F(CircularBufferTest, InsertTest) {
    buffer.set_capacity(3);

    buffer.push_back('A');
    buffer.push_back('C');

    buffer.insert(1, 'B');

    EXPECT_EQ(buffer.size(), 3);
    EXPECT_EQ(buffer.front(), 'A');
    EXPECT_EQ(buffer.at(1), 'B');
    EXPECT_EQ(buffer.back(), 'C');
}

TEST_F(CircularBufferTest, EraseTest) {
    buffer.set_capacity(4);

    buffer.push_back('A');
    buffer.push_back('B');
    buffer.push_back('C');
    buffer.push_back('D');

    buffer.erase(1, 3);     // elements erased: 'B', 'C'

    EXPECT_EQ(buffer.size(), 2);
    EXPECT_EQ(buffer.front(), 'A');
    EXPECT_EQ(buffer.back(), 'D');
}

TEST_F(CircularBufferTest, ClearTest) {
    buffer.set_capacity(2);

    buffer.push_back('A');
    buffer.push_back('B');

    buffer.clear();

    EXPECT_EQ(buffer.size(), 0);
    EXPECT_TRUE(buffer.empty());
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);       // Initialize GoogleTest
    return RUN_ALL_TESTS();                     // Run all the tests (0 if success)
}

