#pragma once

#include "SoundProcessor.h"

#include <iostream>
#include <fstream>

#include <vector>
#include <string>

// help file for -h option
void ShowHelp(const std::string& helper_file) {
    std::ifstream file(helper_file);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open help file: " + helper_file);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }
}
