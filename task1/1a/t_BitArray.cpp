#include <gtest/gtest.h>        // Google Test header
#include "BitArray.h"

TEST(BitArrayTest, ConstructorDefaultTest) {
    BitArray a;                 // Create an empty array a
    EXPECT_EQ(a.size(), 0);     // Check that the size is 0
    EXPECT_TRUE(a.empty());     // Check that the array is empty
}

TEST(BitArrayTest, ConstructorWithSizeTest) {
    BitArray a(5, 19);          // Create an array a with 5 bits and value 19 (binary: 10011)
    EXPECT_EQ(a.size(), 5);     // Check that the size is 5
    EXPECT_EQ(a.to_string(), "11001"); // Check string representation
}

TEST(BitArrayTest, CopyConstructorTest) {
    BitArray a(5, 19);
    BitArray b(a);              // Create a copy of array a
    EXPECT_EQ(a, b);            // Check that the copy is equal to the original
}

TEST(BitArrayTest, SwapTest) {
    BitArray a(5, 19);          // Create an array a with 5 bits and value 19 (binary: 10011)
    BitArray b(5, 27);          // Create an array b with 5 bits and value 27 (binary: 11011)
    a.swap(b);
    EXPECT_EQ(a.to_string(), "11011"); 
    EXPECT_EQ(b.to_string(), "11001");
}

TEST(BitArrayTest, AssignnmentOperatorTest) {
    BitArray a(5, 19);
    BitArray b = a;             // Assign a to b;
    EXPECT_EQ(a, b);            // Check that the copy is equal to the original
}

TEST(BitArrayTest, ResizeTest) {
    BitArray a(5, 19);
    a.resize(7, true);
    EXPECT_EQ(a.size(), 7);                 // Check the new size
    EXPECT_EQ(a.to_string(), "1100111");    // Check the new value
    a.resize(3, false);
    EXPECT_EQ(a.size(), 3);
    EXPECT_EQ(a.to_string(), "110");
}

TEST(BitArrayTest, ClearTest) {
    BitArray a(5, 19);
    a.clear();
    EXPECT_EQ(a.size(), 0);     // Check that the new size is 0
    EXPECT_TRUE(a.empty());     // Check that the array is empty
}

TEST(BitArrayTest, PushBackTest) {
    BitArray a(5, 19);
    a.push_back(true);
    EXPECT_EQ(a.size(), 6);
    EXPECT_EQ(a.to_string(), "110011");    // Check the new value
}

TEST(BitArrayTest, ANDOperatorTest) {
    BitArray a(5, 19);          // 10011
    BitArray b(5, 27);          // 11011
    a &= b;                     // Perform bitwise AND operation
    EXPECT_EQ(a.to_string(), "11001"); 
}

TEST(BitArrayTest, OROperatorTest) {
    BitArray a(5, 19);
    BitArray b(5, 27);
    a |= b;
    EXPECT_EQ(a.to_string(), "11011"); 
}

TEST(BitArrayTest, XOROperatorTest) {
    BitArray a(5, 19);
    BitArray b(5, 27);
    a ^= b;
    EXPECT_EQ(a.to_string(), "00010"); 
}

TEST(BitArrayTest, LeftShiftOperatorTest) {
    BitArray a(5, 19);          // 10011
    a <<= 1;
    EXPECT_EQ(a.to_string(), "10010");
}

TEST(BitArrayTest, RightShiftOperatorTest) {
    BitArray a(5, 19);          // 10011
    a >>= 1;
    EXPECT_EQ(a.to_string(), "01100");
}

TEST(BitArrayTest, SetTests) {
    BitArray a(5, 19);                      // 10011
    a.set(2, true);                         // Set the 2nd bit
    EXPECT_EQ(a.to_string(), "11101"); 
    a.set();                                // Set all bits
    EXPECT_EQ(a.to_string(), "11111");
}

TEST(BitArrayTest, ResetTest) {
    BitArray a(5, 19);
    a.reset(0);
    EXPECT_EQ(a.to_string(), "01001");
    a.reset();
    EXPECT_EQ(a.to_string(), "00000");
}

TEST(BitArrayTest, AnyTest) {
    BitArray a(5, 0);
    EXPECT_FALSE(a.any());
    a.set(0, true);                        
    EXPECT_TRUE(a.any());
}

TEST(BitArrayTest, NoneTest) {
    BitArray a(5, 0);
    EXPECT_TRUE(a.none());
    a.set(0, true);                        
    EXPECT_FALSE(a.none());
}

TEST(BitArrayTest, InversionOperatorTest) {
    BitArray a(5, 19);          // 10011
    BitArray b = ~a;
    EXPECT_EQ(b.to_string(), "00110");
}

TEST(BitArrayTest, CountTest) {
    BitArray a(5, 19);
    EXPECT_EQ(a.count(), 3);
}

TEST(BitArrayTest, ElementAccessOperatorTest) {
    BitArray a(5, 19);
    EXPECT_TRUE(a[0]);
    EXPECT_TRUE(a[1]);
    EXPECT_FALSE(a[2]);
    EXPECT_FALSE(a[3]);
    EXPECT_TRUE(a[4]);
}

TEST(BitArrayTest, SizeTest) {
    BitArray a(5, 19);
    EXPECT_EQ(a.size(), 5);
}

TEST(BitArrayTest, EmptyTest) {
    BitArray a;                 // Create an empty array
    EXPECT_TRUE(a.empty());     // Check that the array is empty
    a.push_back(true);          // Add 1 set bit to the array
    EXPECT_FALSE(a.empty());    // Check that the array is not empty
}

TEST(BitArrayTest, ToStringTest) {
    BitArray a(5, 19);
    EXPECT_EQ(a.to_string(), "11001");
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);       // Initialize GoogleTest
    return RUN_ALL_TESTS();                     // Run all the tests (0 if success)
}
// https://www.youtube.com/watch?v=16FI1-d2P4E