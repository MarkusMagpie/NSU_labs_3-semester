#include "SoundProcessor.h"

#include <iostream>
#include <fstream>

#include <vector>
#include <string>

// help file for -h option
void ShowHelp(const std::string& helper_file) {
    std::ifstream file(helper_file);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open help file: " << helper_file << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Error: Insufficient arguments.\n";
        ShowHelp("help.txt");
        return 0;
    }

    std::string output_file;
    std::string config_file;
    std::vector<std::string> input_files;

    if (std::string(argv[1]) == "-h") {
        ShowHelp("help.txt");
        // return 0;
    }

    config_file = argv[1];
    output_file = argv[2];

    if (argc < 4) {
        std::cerr << "Error: Missing input files.\n";
        ShowHelp("help.txt");
        return 0;
    }

    for (int i = 3; i < argc; ++i) {
        input_files.push_back(argv[i]);
    }

    try {
        SoundProcessor processor(output_file, input_files, config_file);
        processor.MainProcess();
        std::cout << "Processing of reading input WAV files, conversion and writing completed successfully" << std::endl;
        std::cout << "Output WAV file: " << output_file << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl; // no idea what else to check so TODO
        return 0;
    }

    return 0;
}
