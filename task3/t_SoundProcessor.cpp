#include <gtest/gtest.h>
#include "ConfigParser.h"

// Test that checks if the correct exception is thrown when a non-existent file is passed
TEST(ConfigParserTest, ThrowsExceptionForNonExistentFile) {
    std::string config_file = "non-existent-file.txt";
    std::vector<std::string> input_files = {"input_file1.wav", "input_file2.wav"};
    ConfigParser parser(config_file, input_files);
    EXPECT_THROW(parser.Parse(), std::runtime_error);
}

int main(void) {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}