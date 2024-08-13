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
public:
    WAVFileWriter(std::string& filename, int sample_rate, int num_samples);
    ~WAVFileWriter();
    
    void WriteHeader();
    void WriteSamples(std::vector<int16_t>& samples);
};
