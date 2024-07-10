#pragma once

#include "StrategyFactory.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

class PrisonersDilemmaSimulator {
private: 
    std::vector<std::unique_ptr<Strategy>> strategies; // a
    int steps; // c
    std::vector<std::vector<int>> matrix; // e

    std::vector<std::vector<char>> histories;
    std::vector<int> scores;
public:
    // constructor: names of competing strategies (3); number of steps; matix file
    PrisonersDilemmaSimulator(const std::vector<std::string>& names, int steps, const std::string& matrix_file) : steps(steps) 
    {
        for (const auto name : names) {
            strategies.push_back(StrategyFactory::GetInstance().CreateStrategy(name));
        }
        LoadMatrix(matrix_file);
    }

    void Run(bool detailed = false) {
        Reset();
        for (int step = 0; step < steps; ++step) {
            std::vector<char> moves;
            for (auto& strategy : strategies) {
                moves.push_back(strategy->MakeMove(histories[0], histories[1], histories[2]));
            }
            UpdateScores(moves);
            UpdateHistories(moves);
            if (detailed) {
                PrintStepResults(step, moves);
            }
        }
        PrintFinalResults();
    }

    void LoadMatrix(const std::string& matrix_file) {
        std::ifstream input(matrix_file);
        if (!input.is_open()) {
            throw std::runtime_error("Failed to open file: " + matrix_file);
        }

        std::string line;
        while (std::getline(input, line)) {
            std::vector<int> row;
            std::string word;
            for (char c : line) {
                if (c == ' ') {
                    row.push_back(std::stoi(word)); // stoi - string to int
                    word.clear();
                } else {
                    word += c;
                }
            }
            row.push_back(std::stoi(word));
            matrix.push_back(row);
        }
    }

    void Reset() {
        scores.assign(strategies.size(), 0);
        histories.assign(strategies.size(), std::vector<char>());
        for (auto& strategy : strategies) {
            strategy->reset();
        }
    }

    void UpdateScores(const std::vector<char>& moves) {
        // each move setup of a strategy has index in this range: [0, 7].
        // example: ['C', 'D', 'C'] => index = 0 * 4 + 1 * 2 + 0 = 2
        // ccc = 0; ccd = 1; cdc = 2; cdd = 3, ...

        // matrix example: matrix[2][3] = {{3,3,3}, {0,5,5}}

        // VERSION FOR 3 STRATEGIES ONLY
        // int index = (moves[0] == 'C' ? 0 : 1) * 4 + 
        // (moves[1] == 'C' ? 0 : 1) * 2 +
        // (moves[2] == 'C' ? 0 : 1);
        int index = 0;
        for (int i = 0; i < moves.size(); ++i) {
            if (moves[i] == 'C') {
                index += 1 << i;
            }
        }

        // update scores
        for (int i = 0; i < strategies.size(); ++i) {
            scores[i] += matrix[index][i];
        }
    }

    void UpdateHistories(const std::vector<char>& moves) {
        for (int i = 0; i < strategies.size(); ++i) {
            histories[i].push_back(moves[i]);
        }
    }

    void PrintStepResults(int step, const std::vector<char>& moves) {
        std::cout << "Step " << step + 1 << ":\n";
        for (int i = 0; i < strategies.size(); ++i) {
            std::cout << "Strategy " << i + 1 << " chose " << moves[i] << "\n";
        }
        std::cout << "Scores: ";
        for (int i = 0; i < strategies.size(); ++i) {
            std::cout << scores[i] << " ";
        }
        std::cout << "\n\n";
    }

    void PrintFinalResults() {
        std::cout << "Final scores: \n";
        for (int i = 0; i < strategies.size(); ++i) {
            std::cout << "Strategy " << i + 1 << ": " << scores[i] << "\n";
        }
    }
};