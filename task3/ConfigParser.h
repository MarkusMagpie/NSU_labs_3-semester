#pragma once

#include "Converter.h"
#include "MixConverter.h"
#include "MuteConverter.h"
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
    std::vector<std::unique_ptr<Converter>> converters;
public:
    ConfigParser(const std::string& config_file);
    void Parse(); // method to parse config file by parsing line by line
    std::unique_ptr<Converter> CreateConverter(const std::string& line);
    std::string ReadLine(std::ifstream& file);
    std::vector<int16_t> LoadStreamFromFile(const std::string& file_name);

    const std::vector<std::unique_ptr<Converter>>& GetConverters(); // getter
};