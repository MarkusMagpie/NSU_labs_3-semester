#pragma once

#include "ConfigParser.h"

#include <vector>
#include <string>

class SoundProcessor {
public:
    SoundProcessor(const std::string& output_file, const std::vector<std::string>& input_files, const std::string& config_file);
    void MainProcess(); // Reaad all WAV files; parse config; convert and write output WAV file
private:
    std::string output_file;
    std::vector<std::string> input_files;
    std::string config_file;
};