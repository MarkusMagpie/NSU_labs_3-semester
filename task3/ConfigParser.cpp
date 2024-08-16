#include "ConfigParser.h"

#include <fstream> 
#include <sstream> // for std::istringstream
#include <iostream>

ConfigParser::ConfigParser(const std::string& config_file, const std::vector<std::string>& input_files) : 
    config_file(config_file), input_files(input_files), sample_rate(44100) {}

void ConfigParser::Parse() {
    std::ifstream file(config_file);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open config file: " + config_file);
    }

    std::string line;
    std::unique_ptr<Converter> converter;

    while (std::getline(file, line)) {
        line.erase(0, line.find_first_not_of(" \t")); // remove leading whitespace (if any)
        line.erase(line.find_last_not_of(" \t") + 1); // remove trailing whitespace (if any)

        if (line.empty() || line[0] == '#') {
            continue; // skip empty lines or comments
        }

        converter = CreateConverter(line);
        if (converter) {
            converters.push_back(std::move(converter));
        } else {
            throw std::runtime_error("Unknown config line: " + line);
        } 
    }
}

std::unique_ptr<Converter> ConfigParser::CreateConverter(const std::string& line) {
    std::istringstream stream(line);
    std::string command;
    stream >> command;

    if (command == "mute") {
        double start_time, end_time;
        if (stream >> start_time >> end_time) {
            return std::make_unique<MuteConverter>(start_time, end_time, sample_rate);
        }
    } else if (command == "mix") {
        std::string input_file_ref;
        double insert_time;
        if (stream >> input_file_ref >> insert_time) {
            int input_file_index = ParseInputFileReference(input_file_ref);
            if (input_file_index != -1) {
                std::vector<int16_t> second_stream = LoadStreamFromFile(input_file_index);
                return std::make_unique<MixConverter>(second_stream, insert_time, sample_rate);
            } else {
                throw std::runtime_error("Invalid input file reference: " + input_file_ref);
            }
        }
    }

    return nullptr;
}

int ConfigParser::ParseInputFileReference(const std::string& ref) {
    if (ref.size() > 1 && ref[0] == '$') {
        std::string number = ref.substr(1);
        for (char c : number) {
            if (!std::isdigit(c)) {
                return -1;
            }
        }
        int index = std::stoi(number);
        if (index >= 1 && index <= input_files.size()) {
            return index - 1;
        }
    }

    return -1;
}

std::vector<int16_t> ConfigParser::LoadStreamFromFile(int file_index) {
    if (file_index < 0 || file_index >= input_files.size()) {
        throw std::runtime_error("Input file index out of range (given wrong value): " + std::to_string(file_index));
    }

    const std::string& file_name = input_files[file_index];
    std::cout << "Using reader to get samples from file: " << file_name << std::endl;
    WAVFileReader reader(file_name);
    return reader.ReadSamples();
}

const std::vector<std::unique_ptr<Converter>>& ConfigParser::GetConverters() {
    return converters;
}