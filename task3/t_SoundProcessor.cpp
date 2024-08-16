#include <gtest/gtest.h>

#include "ConfigParser.h"
#include "WAVFileReader.h"
#include "WAVFileWriter.h"

// 3 WAVFileReader tests
TEST(WAVFileReaderTest, NonExistentReadinfFileTest) {
    std::string non_existent_file = "/non_existent_directory/non_existent.wav";
    EXPECT_THROW(WAVFileReader reader(non_existent_file), std::runtime_error);
}

void CreateInvalidWAVFile(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    
    if (file.is_open()) {
        WAVHeader invalid_header;
        invalid_header.audioFormat = 2;  // 1 for PCM - must cause error!
        invalid_header.numChannels = 1;
        invalid_header.bitsPerSample = 16;
        invalid_header.sample_rate = 44100;

        file.write(reinterpret_cast<const char*>(&invalid_header), sizeof(WAVHeader));
        file.close();
    }
}

TEST(WAVFileReaderTest, InvalidHeaderTest) {
    std::string invalid_wav_file = "invalid_header.wav";
    CreateInvalidWAVFile(invalid_wav_file);
    
    EXPECT_THROW(WAVFileReader reader(invalid_wav_file), std::runtime_error);
}

void CreateWAVFileWithoutSamples(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        WAVHeader header;
        header.audioFormat = 1;
        header.numChannels = 1;
        header.bitsPerSample = 16;
        header.sample_rate = 44100;
        header.dataSize = 100;  // must cause error!

        file.write(reinterpret_cast<const char*>(&header), sizeof(WAVHeader));
        file.close();
    }
}

TEST(WAVFileReaderTest, ReadSamplesTest) {
    std::string wav_file_without_samples = "no_samples.wav";
    CreateWAVFileWithoutSamples(wav_file_without_samples);
    
    WAVFileReader reader(wav_file_without_samples);
    EXPECT_THROW(reader.ReadSamples(), std::runtime_error);
}

// 4 ConfigParse tests
TEST(ConfigParserTest, NonExistentConfigFileTest) {
    std::string config_file = "non_existent_file.txt";
    std::vector<std::string> input_files = {"input_file1.wav", "input_file2.wav"};
    ConfigParser parser(config_file, input_files);
    
    EXPECT_THROW(parser.Parse(), std::runtime_error);
}

TEST(ConfigParserTest, ErrorMessageForNonExistentConfigFileTest) {
    std::string config_file = "non_existent_file.txt";
    std::vector<std::string> input_files = {"input_file1.wav", "input_file2.wav"};
    ConfigParser parser(config_file, input_files);

    try {
        parser.Parse();
    } catch (const std::runtime_error& e) {
        EXPECT_STREQ("Failed to open config file: non_existent_file.txt", e.what());
    }
}

TEST(ConfigParserTest, NonExistentConverterLineTest) {
    std::ofstream temp_config("temp_config.txt"); // created temp file "temp_config.txt"; wrote "uaaaa 0 5" in it and closed
    temp_config << "uaaaaxdrge 0 666" << std::endl;
    temp_config.close();

    std::vector<std::string> input_files = {"input1.wav", "input2.wav"};
    ConfigParser parser("temp_config.txt", input_files);

    EXPECT_THROW(parser.Parse(), std::runtime_error);
}

TEST(ConfigParserTest, ErrorMessageForNonExistentConverterLineTest) {
    std::ofstream temp_config("temp_config.txt");
    temp_config << "uaaaaxdrge 0 666" << std::endl;
    temp_config.close();

    std::vector<std::string> input_files = {"input1.wav", "input2.wav"};
    ConfigParser parser("temp_config.txt", input_files);

    try {
        parser.Parse();
    } catch (const std::runtime_error& e) {
        EXPECT_STREQ("Unknown config line: uaaaaxdrge 0 666", e.what());
    }
}

// 2 WAVFileWriter tests
TEST(WAVFileWriterTest, NonExistentWritingFileTest) {
    std::string non_existent_file = "/non_existent_directory/non_existent.wav";
    int sample_rate = 44100;
    int num_samples = 100;

    EXPECT_THROW(WAVFileWriter writer(non_existent_file, sample_rate, num_samples), std::runtime_error);
}

TEST(WAVFileWriterTest, WrongSampleCountTest) {
    std::string filename = "test_output.wav";
    int sample_rate = 44100;
    int num_samples = 100;

    WAVFileWriter writer(filename, sample_rate, num_samples);

    std::vector<int16_t> mismatched_samples(num_samples + 10);

    EXPECT_THROW(writer.WriteSamples(mismatched_samples), std::runtime_error);
}

int main(void) {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}