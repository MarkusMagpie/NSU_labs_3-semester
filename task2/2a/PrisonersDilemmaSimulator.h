#pragma once

#include "StrategyFactory.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

class PrisonersDilemmaSimulator {
private: 
    std::vector<std::unique_ptr<Strategy>> strategies; // a
    int steps; // c
    std::vector<std::vector<int>> matrix; // e

    std::vector<std::vector<char>> histories;
    std::vector<int> scores;
public:
    // constructor: names of competing strategies (3); number of steps; configuration directory, matix file
    PrisonersDilemmaSimulator(const std::vector<std::string>& names, int steps, const std::string& matrix_file, const std::string& config_dir = "") : 
        steps(steps) 
    {
        for (const auto &name : names) {
            strategies.push_back(StrategyFactory::GetInstance().CreateStrategy(name));
        }
        if (config_dir != "") {
            LoadStrategyConfigs(config_dir);
        } else {
            std::cout << "No configuration directory provided" << std::endl;
        }
        LoadMatrix(matrix_file);
    }

    void Run(bool detailed = false) {
        Reset();
        for (int step = 0; step < steps; ++step) {
            std::vector<char> moves;
            for (int i = 0; i < strategies.size(); ++i) {
                std::vector<char> opp1_history = histories[(i + 1) % strategies.size()];
                std::vector<char> opp2_history = histories[(i + 2) % strategies.size()];
                moves.push_back(strategies[i]->MakeMove(histories[i], opp1_history, opp2_history));
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
        std::cout << "Loading matrix from: " << matrix_file << std::endl;
        std::ifstream input(matrix_file);
        if (!input.is_open()) {
            throw std::runtime_error("Failed to open matrix file: " + matrix_file);
        }

        std::string line;
        while (std::getline(input, line)) {
            std::stringstream temp(line);
            std::vector<int> row;
            int num;
            while (temp >> num) {
                row.push_back(num);
            }
            if (!row.empty()) {
                matrix.push_back(row);
            }
        }
        std::cout << "Matrix loaded successfully" << "\n" << std::endl;
    }

    void LoadStrategyConfigs(const std::string& config_dir) {
        std::cout << "Loading strategy configs from: " << config_dir << std::endl;
        for (const auto& strategy : strategies) {
            std::string config_file = config_dir + "/" + strategy->GetName() + ".txt";
            std::ifstream input(config_file);
            if (!input.is_open()) {
                throw std::runtime_error("Failed to open config file: " + config_file);
            }

            std::string line;
            while (std::getline(input, line)) {
                if (line.empty() || line[0] == '#') {
                    continue; // skip empty lines or comments
                }

                // parse pairs of key/value
                // https://cplusplus.com/forum/general/266880/
                auto delimiterPos = line.find("=");
                auto key = line.substr(0, delimiterPos);
                auto value = line.substr(delimiterPos + 1);
                strategy->SetConfig(key, value);

                std::cout << "Loaded config: " << key << " = " << value << std::endl;
            }
            input.close();
        }
        std::cout << "Strategy configs loaded successfully" << "\n" << std::endl;
    }

    const std::vector<int>& GetScores() const {
        return scores;
    }

    const std::vector<std::vector<char>>& GetHistories() const {
        return histories;
    }

    void Reset() {
        scores.assign(strategies.size(), 0);
        histories.assign(strategies.size(), std::vector<char>());
    }

    void UpdateScores(const std::vector<char>& moves) {
        // each move setup of a strategy has index in this range: [0, 7].
        // example: ['C', 'D', 'C'] => index = 0 * 4 + 1 * 2 + 0 = 2
        // ccc = 0; ccd = 1; cdc = 2; cdd = 3, ...

        // VERSION FOR 3 STRATEGIES ONLY
        // int index = (moves[0] == 'C' ? 0 : 1) * 4 + 
        // (moves[1] == 'C' ? 0 : 1) * 2 +
        // (moves[2] == 'C' ? 0 : 1);
        int index = 0;
        for (int i = 0; i < static_cast<int>(moves.size()); ++i) {
            if (moves[moves.size() - i - 1] == 'D') {
                index += 1 << i;
            }
        }

        // update scores
        // std::cout << "Chosen index: " << index << "\n";

        // std::cout << "Chosen line ";
        // for (auto &elem : matrix[index]) {
        //     std::cout << elem << " ";
        // }
        // std::cout << "\n";

        for (int i = 0; i < static_cast<int>(strategies.size()); ++i) {
            scores[i] += matrix[index][i];
        }
    }

    void UpdateHistories(const std::vector<char>& moves) {
        for (int i = 0; i < static_cast<int>(strategies.size()); ++i) {
            histories[i].push_back(moves[i]);
        }
    }

    void PrintStepResults(int step, const std::vector<char>& moves) {
        std::cout << "Step " << step + 1 << ":\n";
        for (int i = 0; i < static_cast<int>(strategies.size()); ++i) {
            std::cout << "Strategy " << i + 1 << " chose " << moves[i] << "\n";
        }
        std::cout << "Scores: ";
        for (int i = 0; i < static_cast<int>(strategies.size()); ++i) {
            std::cout << scores[i] << " ";
        }
        std::cout << "\n\n";
    }

    void PrintFinalResults() {
        std::cout << "Final scores: \n";
        for (int i = 0; i < static_cast<int>(strategies.size()); ++i) {
            std::cout << "Strategy " << i + 1 << ": " << scores[i] << "\n";
        }
        if ((scores[0] == scores[1]) && (scores[1] == scores[2])) {
            std::cout << "All strategies have the same score!" << std::endl;
            return;
        }
        std::cout << "Strategy with the highest score: " << std::max_element(scores.begin(), scores.end()) - scores.begin() + 1 << "\n";
    }
};