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
    std::vector<std::unique_ptr<Strategy>> strategies;
    int steps;
    std::vector<std::vector<int>> matrix;

    std::vector<std::vector<char>> histories;
    std::vector<int> scores;

    std::map<std::pair<std::string, int>, int> tournament_scores;
    std::map<std::tuple<int, int, int>, std::vector<std::vector<char>>> tournament_histories;
    // key: std::tuple<int, int, int> - combos of strategy indices
    // value: std::vector<std::vector<char>> - histories of each strategy
public:
    // constructor: names of competing strategies (3); number of steps; configuration directory, matix file
    PrisonersDilemmaSimulator(const std::vector<std::string>& names, int steps, const std::string& matrix_file, const std::string& config_dir = "") : 
        steps(steps) 
    {
        for (const auto &name : names) {
            strategies.push_back(StrategyFactory::GetInstance().CreateStrategy(name));
        }
        // AFTER CREATING STRATEGIES ASSIGN DEFAULT VALUES FOR SCORES AND HISTORIES  
        Reset();

        // if (config_dir != "configs") {
        LoadStrategyConfigs(config_dir);
        // } else { std::cout << "No configuration directory provided, using default: configs" << std::endl; }

        // if (matrix_file != "default_matrix.txt") {
        LoadMatrix(matrix_file);
        // } else { std::cout << "No matrix file provided, using default: default_matrix.txt" << std::endl; }
    }

    void Run(bool detailed = true) {
        std::cout << "---------------------------------------------------" << std::endl;
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

    void RunTournament() {
        if (strategies.size() < 3) {
            throw std::runtime_error("Number of strategies must be 3 or more for tournament mode!");
        }

        std::cout << "--------------------------------" << std::endl;
        int round = 1;

        std::cout << "List of indexes and strategies:" << std::endl;
        for (int i = 0; i < strategies.size(); ++i) {
            std::cout << i + 1 << ". " << strategies[i]->GetName() << std::endl;
        } 
        std::cout << std::endl;

        // generate all posible 3s without repetitions
        for (size_t i = 0; i < strategies.size(); ++i) {
            for (size_t j = i + 1; j < strategies.size(); ++j) {
                for (size_t k = j + 1; k < strategies.size(); ++k) {
                    std::vector<std::unique_ptr<Strategy>> cur_strats;
                    cur_strats.push_back(StrategyFactory::GetInstance().CreateStrategy(strategies[i]->GetName()));
                    cur_strats.push_back(StrategyFactory::GetInstance().CreateStrategy(strategies[j]->GetName()));
                    cur_strats.push_back(StrategyFactory::GetInstance().CreateStrategy(strategies[k]->GetName()));

                    for (int step = 0; step < steps; ++step) {
                        std::vector<char> moves;
                        for (auto& strategy : cur_strats) {
                            moves.push_back(strategy->MakeMove(histories[0], histories[1], histories[2]));
                        }
                        UpdateScores(moves);
                        UpdateHistories(moves);
                    }

                    tournament_histories[std::make_tuple(i, j, k)] = histories;
                    
                    
                    tournament_scores[std::make_pair(strategies[i]->GetName(), i)] += scores[0];
                    tournament_scores[std::make_pair(strategies[j]->GetName(), j)] += scores[1];
                    tournament_scores[std::make_pair(strategies[k]->GetName(), k)] += scores[2];
                
                    std::cout << "Tournament round " << round++ << " results: " << std::endl;
                    // std::cout << "(Index. Strategy: Round Score)" << std::endl;
                    std::cout << i + 1 << ". " << strategies[i]->GetName() << ": " << scores[0] << "\n";
                    std::cout << j + 1 << ". " << strategies[j]->GetName() << ": " << scores[1] << "\n";
                    std::cout << k + 1 << ". " << strategies[k]->GetName() << ": " << scores[2] << "\n";

                    std::cout << std::endl;
                    Reset();
                }
            }
        }

        std::cout << "------------------------" << std::endl;
        std::cout << "Tournament final results:" << std::endl;
        for (int i = 0; i < static_cast<int>(strategies.size()); ++i) {
            std::cout << i + 1 << ". " << strategies[i]->GetName() << ": " << tournament_scores[std::make_pair(strategies[i]->GetName(), i)] << std::endl;
        }
        std::cout << std::endl;

        // std::cout << "Histories: " << std::endl;
        // for (const auto& history : histories) {
        //     for (const auto& move : history) {
        //         std::cout << move << " ";
        //     }
        //     std::cout << std::endl;
        // }
        // std::cout << std::endl;

        // equal scores check
        int max_score = std::max_element(tournament_scores.begin(), tournament_scores.end(),
            [](const auto& a, const auto& b) { return a.second < b.second; })->second;

        std::vector<std::pair<std::string, int>> winners;
        for (const auto& entry : tournament_scores) {
            if (entry.second == max_score) {
                winners.push_back(entry.first);
            }
        }

        if (winners.size() == tournament_scores.size()) {
            std::cout << "All tournament strategies have the same score!" << "\n" << std::endl;
            return;
        }

        std::cout << "Strategy(-ies) with the highest score: " << std::endl;
        for (const auto& winner : winners) {
            std::cout << winner.second + 1 << ". " << winner.first << "\n";
        }
        std::cout << std::endl;
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
        std::cout << "\n" << "Loading strategy configs from: " << config_dir << std::endl;
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

    // getters for testing purposes
    const std::vector<int>& GetScores() const {
        return scores;
    }

    const std::vector<std::vector<char>>& GetHistories() const {
        return histories;
    }

    const std::map<std::pair<std::string, int>, int>& GetTournamentScores() const {
        return tournament_scores;
    }

    const std::map<std::tuple<int, int, int>, std::vector<std::vector<char>>>& GetTournamentHistories() const {
        return tournament_histories;
    }

    void Reset() {
        scores.assign(strategies.size(), 0);
        histories.assign(strategies.size(), std::vector<char>());
    }

    void UpdateScores(const std::vector<char>& moves) {
        // each move setup of a strategy has index in this range: [0, 7].
        // example: ['C', 'D', 'C'] => index = 0 * 4 + 1 * 2 + 0 = 2
        // ccc = 0; ccd = 1; cdc = 2; cdd = 3, ...

        int index = 0;
        for (int i = 0; i < static_cast<int>(moves.size()); ++i) {
            if (moves[moves.size() - i - 1] == 'D') {
                index += 1 << i;
            }
        }

        // update scores
        for (int i = 0; i < 3; ++i) {
            // std::cout << "i = " << i << std::endl;
            // std::cout << "Score[" << i << "] = " << scores[i] << std::endl;
            // std::cout << "Matrix[" << index << "][" << i << "] = " << matrix[index][i] << std::endl;
            // std::cout << "Old Score[" << i << "] = " << scores[i] << std::endl;
            scores[i] += matrix[index][i];
            // std::cout << "New Score[" << i << "] = " << scores[i] << std::endl;
        }
    }

    void UpdateHistories(const std::vector<char>& moves) {
        for (int i = 0; i < 3; ++i) {
            // std::cout << "Move[" << i << "] = " << moves[i] << std::endl;
            // std::cout << "History[" << i << "] = " << histories[i].size() << std::endl;
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
        std::cout << "FINAL SCORES: \n";
        for (int i = 0; i < static_cast<int>(strategies.size()); ++i) {
            std::cout << strategies[i]->GetName() << " " << i + 1 << ": " << scores[i] << "\n";
        }
        std::cout << "\n";

        bool all_equal = true;
        for (int i = 1; i < static_cast<int>(strategies.size()); ++i) {
            if (scores[i] != scores[0]) {
                all_equal = false;
                break;
            }
        }

        if (all_equal) {
            std::cout << "All strategies have the same score!" << "\n" <<std::endl;
            return;
        }

        std::cout << "Strategy with the highest score: " << std::max_element(scores.begin(), scores.end()) - scores.begin() + 1 << ".\n";
        std::cout << strategies[std::max_element(scores.begin(), scores.end()) - scores.begin()]->GetName()  << " has won!"<< "\n" << std::endl;
    }
};