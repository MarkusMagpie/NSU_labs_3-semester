#include "ConfigParser.h"

#include <fstream> 
#include <sstream> // for std::istringstream

ConfigParser::ConfigParser(const std::string& config_file) : 
    config_file(config_file), sample_rate(44100) {}

void ConfigParser::Parse() {
    std::ifstream file(config_file);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open config file: " + config_file);
    }

    std::string line;
    std::unique_ptr<Converter> converter;

    while (std::getline(file, line)) {
        line = ReadLine(file);
        if (!line.empty()) {
            converter = CreateConverter(line);
            if (converter) {
                converters.push_back(std::move(converter));
            } else {
                throw std::runtime_error("Unknown config line: " + line);
            }
        } 
    }
}

std::string ConfigParser::ReadLine(std::ifstream& file) {
    std::string line;
    if (std::getline(file, line)) {
        line.erase(0, line.find_first_not_of(" \t")); // remove leading whitespace (if any)
        line.erase(line.find_last_not_of(" \t") + 1); // remove trailing whitespace (if any)

        if (line.empty() || line[0] == '#') {
            return "";
        }
    }

    return line;
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
        std::string file_path;
        double insert_time;
        if (stream >> file_path >> insert_time) {
            std::vector<int16_t> second_stream = LoadStreamFromFile(file_path);
            return std::make_unique<MixConverter>(second_stream, insert_time, sample_rate);
        }
    }

    return nullptr;
}

std::vector<int16_t> ConfigParser::LoadStreamFromFile(const std::string& file_name) {
    WAVFileReader reader(file_name);
    return reader.ReadSamples();
}

const std::vector<std::unique_ptr<Converter>>& ConfigParser::GetConverters() {
    return converters;
}