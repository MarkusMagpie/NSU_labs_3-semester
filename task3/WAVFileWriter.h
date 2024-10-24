#pragma once

#include "WAVHeader.h"

#include <fstream>
#include <string>
#include <vector>
#include <cstring>

class WAVFileWriter {
private:
    std::ofstream output;
    int sample_rate;
    int num_samples;
    int wav_size;
    int data_size;
public:
    WAVFileWriter(std::string& filename, int sample_rate, int num_samples, int wav_size, int data_size);
    ~WAVFileWriter();
    
    void WriteHeader();
    void WriteSamples(std::vector<int16_t>& samples);
};
