#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Rule.h"

class Universe {
private: 
    int width;
    int height;
    int current_iteration;
    std::vector<std::vector<bool>> grid;
    std::string name;
    Rule rule;

    int CountAliveNeighbors(int x, int y) const;
public:
    Universe(int width, int height, const std::vector<std::vector<bool>>& grid);
    void print() const;
    void tick(int iterations = 1);

    // getters
    std::string GetName() const { return name; }
    std::string GetRule() const { return rule.GetRuleString(); }
    std::vector<std::vector<bool>> GetGrid() const { return grid; }
    int GetCurrentIteration() const { return current_iteration; }

    // setters
    void SetName(const std::string& name) { this->name = name; }
    void SetRule(const std::string& rule) { this->rule = Rule(rule); }
};

Universe::Universe(int width, int height, const std::vector<std::vector<bool>>& grid) 
    : width(width), height(height), grid(grid), current_iteration(0) {}

void Universe::print() const {
    for (const auto& row : grid) {
        for (bool cell : row) {
            std::cout << (cell ? "O" : ".");
        }
        std::cout << std::endl;
    }
}

void Universe::tick(int iterations) {
    for (int i = 0; i < iterations; ++i) {
        std::vector<std::vector<bool>> new_grid = grid;
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int live_neighbors = CountAliveNeighbors(x, y);
                if (grid[y][x]) {
                    new_grid[y][x] = rule.IsSurvival(live_neighbors);
                } else {
                    new_grid[y][x] = rule.IsBirth(live_neighbors);
                }
            }
        }

        grid = new_grid;
        ++current_iteration;
    }
}

int Universe::CountAliveNeighbors(int x, int y) const {
    int cnt = 0;
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue;
            int nx = (x + dx + width) % width; // % width in case of (x + dx) < 0 || (x + dx) >= width
            int ny = (y + dy + height) % height;
            // check < 0 || >= height is not needed because nx and ny will be here: [0, width) and [0, height)
            if (grid[ny][nx]) cnt++;
        }
    }

    return cnt;
}