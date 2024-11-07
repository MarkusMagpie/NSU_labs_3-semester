#include <gtest/gtest.h>

#include "main.h"
#include "ConfigParser.h"
#include "WAVFileReader.h"
#include "WAVFileWriter.h"

// 1 main test
TEST(MainTest, MainExceptionTest) {
    EXPECT_THROW(ShowHelp("non_existing_help.txt"), std::runtime_error);
}

// 3 WAVFileReader tests
TEST(WAVFileReaderTest, NonExistentReadinfFileTest) {
    std::string non_existent_file = "/non_existent_directory/non_existent.wav";
    EXPECT_THROW(WAVFileReader reader(non_existent_file), std::runtime_error);

    try {
        WAVFileReader reader(non_existent_file);
    } catch (const std::runtime_error& e) {
        EXPECT_STREQ(e.what(), "Failed to open WAV file for reading: /non_existent_directory/non_existent.wav");
        std::cout << e.what() << std::endl;
    } catch (...) {
        FAIL(); // FAIL means that we don't expect any other exceptions =>  if catch any other exception, it's a fail
    }
}

void CreateInvalidWAVFile(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    
    if (file.is_open()) {
        WAVHeader invalid_header;
        invalid_header.audioFormat = 2;  // 1 for PCM - must cause error!
        invalid_header.numChannels = 1;
        invalid_header.bitsPerSample = 16;

        file.write(reinterpret_cast<const char*>(&invalid_header), sizeof(WAVHeader));
        file.close();
    }
}

TEST(WAVFileReaderTest, InvalidHeaderTest) {
    std::string invalid_wav_file = "invalid_header.wav";
    CreateInvalidWAVFile(invalid_wav_file);
    
    try {
        WAVFileReader reader(invalid_wav_file);
    } catch (const std::runtime_error& e) {
        EXPECT_STREQ(e.what(), "Wrong WAV file format: must be PCM, 16-bit and mono audio");
        std::cout << e.what() << std::endl;
    } catch (...) {
        FAIL();
    }
}

void CreateWAVFileWithoutSamples(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        WAVHeader header;
        WAVChunkBase chunk;
        header.audioFormat = 1;
        header.numChannels = 1;
        header.bitsPerSample = 16;
        header.sample_rate = 44100;
        chunk.chunksize = 100;  // must cause error!

        file.write(reinterpret_cast<const char*>(&header), sizeof(WAVHeader));
        file.close();
    }
}

TEST(WAVFileReaderTest, ReadSamplesTest) {
    std::string wav_file_without_samples = "no_samples.wav";
    CreateWAVFileWithoutSamples(wav_file_without_samples);
    
    WAVFileReader reader(wav_file_without_samples);
    
    try {
        reader.ReadSamples();
    } catch (const std::runtime_error& e) {
        EXPECT_STREQ(e.what(), "Failed to read WAV samples from file");
        std::cout << e.what() << std::endl;
    } catch (...) {
        FAIL();
    }
}

// 2 ConfigParse tests
TEST(ConfigParserTest, ErrorMessageForNonExistentConfigFileTest) {
    std::string config_file = "non_existent_file.txt";
    std::vector<std::string> input_files = {"input_file1.wav", "input_file2.wav"};
    ConfigParser parser(config_file, input_files);

    try {
        parser.Parse();
    } catch (const std::runtime_error& e) {
        EXPECT_STREQ("Failed to open config file: non_existent_file.txt", e.what());
    } catch (...) {
        FAIL();
    }
}

TEST(ConfigParserTest, ErrorMessageForNonExistentConverterLineTest) {
    std::ofstream temp_config("temp_config.txt"); // created temp file "temp_config.txt"; wrote "uaaaaxdrge 0 666" in it and closed
    temp_config << "uaaaaxdrge 0 666" << std::endl;
    temp_config.close();

    std::vector<std::string> input_files = {"input1.wav", "input2.wav"};
    ConfigParser parser("temp_config.txt", input_files);

    try {
        parser.Parse();
    } catch (const std::runtime_error& e) {
        EXPECT_STREQ("Unknown command: uaaaaxdrge", e.what());
    } catch (...) {
        FAIL();
    }
}

// 2 WAVFileWriter tests
TEST(WAVFileWriterTest, NonExistentWritingFileTest) {
    std::string non_existent_file = "/non_existent_directory/non_existent.wav";
    int sample_rate = 44100;
    int num_samples = 100;


    try {
        WAVFileWriter writer(non_existent_file, sample_rate, num_samples, 0, 0);
    } catch (const std::runtime_error& e) {
        EXPECT_STREQ(e.what(), "Failed to open WAV file for writing: /non_existent_directory/non_existent.wav");
        std::cout << e.what() << std::endl;
    } catch (...) {
        FAIL();
    }
}

TEST(WAVFileWriterTest, WrongSampleCountTest) {
    std::string filename = "test_output.wav";
    int sample_rate = 44100;
    int num_samples = 100;

    WAVFileWriter writer(filename, sample_rate, num_samples, 0, 0);

    std::vector<int16_t> mismatched_samples(num_samples + 10); // в параметрах функции WriteSamples - на 10 больше сэмплов чем ожидается

    try {
        writer.WriteSamples(mismatched_samples);
    } catch (const std::runtime_error& e) {
        EXPECT_STREQ(e.what(), "Number of samples does not match the expected count");
        std::cout << e.what() << std::endl;
    } catch (...) {
        FAIL();
    }
}

int main(void) {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}