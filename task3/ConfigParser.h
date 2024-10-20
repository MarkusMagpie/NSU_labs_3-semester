#pragma once

#include "Converter.h"
#include "MixConverter.h"
#include "MuteConverter.h"
#include "AmplifyConverter.h"

#include "WAVFileReader.h" // for LoadStreamFromFile

#include <string>
#include <vector>
#include <memory> // for std::unique_ptr
#include <stdexcept> // for std::runtime_error

// Класс для разбора конфигурационного файла и создания конвертеров
class ConfigParser {
private:
    std::string config_file;
    int sample_rate;
    std::vector<std::string> input_files;
    std::vector<std::unique_ptr<Converter>> converters;
public:
    ConfigParser(const std::string& config_file, const std::vector<std::string>& input_files);
    void Parse(); // method to parse config file by parsing line by line

    std::unique_ptr<Converter> CreateConverter(const std::string& line);
    int ParseInputFileReference(const std::string& ref);
    
    std::vector<int16_t> LoadStreamFromFile(int file_index);
    const std::vector<std::unique_ptr<Converter>>& GetConverters();
};