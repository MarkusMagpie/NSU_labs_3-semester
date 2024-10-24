#pragma once

#include "WAVHeader.h"

#include <fstream>
#include <string>
#include <vector>

class WAVFileReader {
private:
    std::ifstream input;
    int sample_rate;
    int num_samples;

    int wav_size;
    int data_size;
public:
    WAVFileReader(const std::string& filename);
    ~WAVFileReader();
    
    void ReadHeader();
    std::vector<int16_t> ReadSamples();
    int GetSampleRate();
    int GetNumSamples();
    
    int GetWavSize();
    int GetDataSize();
};