#include "main.h"

int main(int argc, char* argv[]) {
    if (argc < 4) {
        ShowHelp("help.txt");
        return 0;
    }

    std::string output_file;
    std::string config_file;
    std::vector<std::string> input_files;

    if (std::string(argv[1]) == "-h") {
        ShowHelp("help.txt");
        return 0;
    }

    config_file = argv[1];
    output_file = argv[2];

    for (int i = 3; i < argc; ++i) {
        input_files.push_back(argv[i]);
    }

    try {
        SoundProcessor processor(output_file, input_files, config_file);
        processor.MainProcess();
        std::cout << "\nProcessing of reading input WAV files, conversion and writing completed successfully" << std::endl;
        std::cout << "Output WAV file: " << output_file << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 0;
    }

    return 0;
}
