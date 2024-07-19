#pragma once

#include "Universe.h"

#include <fstream>

class FileHandler {
private:
    std::vector<std::vector<bool>> ParseState(int width, int height, const std::vector<std::string>& lines);
public: 
    Universe LoadFromFile(const std::string& filename);
    void SaveToFile(Universe& universe, const std::string& filename);
};

// method to read input file "Life 1.06"
// https://conwaylife.com/wiki/Life_1.06
Universe FileHandler::LoadFromFile(const std::string& filename) {
    std::ifstream input(filename);
    if (!input.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::string line;
    std::getline(input, line);
    if (line.substr(0, 4) != "Life") {
        throw std::runtime_error("Invalid file format");
    }

    std::getline(input, line); // #N name
    std::string name = line.substr(3);

    std::getline(input, line); // #R rule
    std::string rule = line.substr(3);

    std::vector<std::string> lines;
    while (std::getline(input, line)) {
        if (line.empty() || line[0] == '#') { // skip empty
            continue;
        }
        lines.push_back(line);
    }

    // count width, height, initial state and create Universe
    int width = 0;
    int height = lines.size();
    for (const std::string& row : lines) {
        if (row.length() > width) {
            width = row.length(); // max width
        }
    }

    // std::cout << "width: " << width << ", height: " << height << std::endl;

    std::vector<std::vector<bool>> init_state = ParseState(width, height, lines);

    Universe universe(width, height, init_state);
    universe.SetName(name);
    universe.SetRule(rule);
    
    return universe;
}

// save universe to file in "Life 1.06" format
void FileHandler::SaveToFile(Universe& universe, const std::string& filename) {
    std::ofstream output(filename);
    if (!output.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    output << "Life 1.06\n";
    output << "#N " << universe.GetName() << "\n";
    output << "#R " << universe.GetRule() << "\n";

    for (const auto& row : universe.GetGrid()) {
        for (bool cell : row) {
            output << (cell ? "O" : ".");
        }
        output << "\n";
    }
}

std::vector<std::vector<bool>> FileHandler::ParseState(int width, int height, const std::vector<std::string>& lines) {
    std::vector<std::vector<bool>> state(height, std::vector<bool>(width, false)); // create state with all false
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            state[y][x] = (lines[y][x] == 'O');
        }
    }
    return state;
} 