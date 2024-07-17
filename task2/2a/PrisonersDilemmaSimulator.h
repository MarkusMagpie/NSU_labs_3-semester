#pragma once

#include "StrategyFactory.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip> // for setw in RunDeatiled

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
    PrisonersDilemmaSimulator(const std::vector<std::string>& names, int steps, const std::string& matrix_file = "default_matrix.txt", const std::string& config_dir = "configs") : 
        steps(steps) 
    {
        for (const auto &name : names) {
            strategies.push_back(StrategyFactory::GetInstance().CreateStrategy(name));
        }
        // AFTER CREATING STRATEGIES ASSIGN DEFAULT VALUES FOR SCORES AND HISTORIES  
        Reset();
        
        if (config_dir.empty()) {
            std::string base_config = "configs";
            LoadStrategyConfigs(base_config);
        } else {
            LoadStrategyConfigs(config_dir);
        }

        LoadMatrix(matrix_file);
    }

    void Run(bool descriptions = false) {
        if (descriptions) { 
            std::cout << "-----LONG MODE-----" << std::endl;
        } else {
            std::cout << "-----FAST MODE-----" << std::endl;
        }
        if (strategies.size() != 3) {
            throw std::runtime_error("Number of strategies must be 3 for fast (or long) mode!");
        }

        for (int step = 0; step < steps; ++step) {
            std::vector<char> moves;
            for (int i = 0; i < strategies.size(); ++i) {
                std::vector<char> opp1_history = histories[(i + 1) % strategies.size()];
                std::vector<char> opp2_history = histories[(i + 2) % strategies.size()];
                moves.push_back(strategies[i]->MakeMove(histories[i], opp1_history, opp2_history));
            }
            UpdateScores(moves);
            UpdateHistories(moves);
            if (descriptions) {
                PrintStepResults(step, moves);
            }
        }

        std::cout << "-------------------" << std::endl;
        PrintFinalResults();
    }

    void RunDetailed() {
        std::cout << "-----DETAILED MODE-----" << std::endl;
        if (strategies.size() != 3) {
            throw std::runtime_error("Number of strategies must be 3 for detailed mode!");
        }
    
        for (int step = 0; step < steps; ++step) {
            std::cout << "Press Enter to proceed to the next step or type 'quit' to exit: ";
            std::string input;
            std::getline(std::cin, input);
            if (input == "quit") {
                std::cout << "Simulation interrupted by user.\n" << std::endl;
                break;
            }

            std::vector<char> moves;
            for (auto& strategy : strategies) {
                moves.push_back(strategy->MakeMove(histories[0], histories[1], histories[2]));
            }

            UpdateScores(moves);
            UpdateHistories(moves);
            
            std::cout << "Step " << step + 1 << " results:" << std::endl;
            int max_name_len = 0;
            for (const auto& strategy : strategies) {
                max_name_len = std::max(max_name_len, static_cast<int>(strategy->GetName().length()));
            }

            for (size_t i = 0; i < strategies.size(); ++i) {
                std::cout << std::setw(max_name_len) << std::left;
                std::cout << strategies[i]->GetName() << ": " << moves[i] << "; score: " << scores[i] << "\n";
            }
        }
        
        std::cout << "-----------------------" << std::endl;
        PrintFinalResults();
    }

    void RunTournament() {
        std::cout << "-----TOURNAMENT MODE-----" << std::endl;
        if (strategies.size() < 3) {
            throw std::runtime_error("Number of strategies must be 3 or more for tournament mode!");
        }

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
        PrintFinalResults();
    }
        

    void LoadMatrix(const std::string& matrix_file) {
        std::cout << "Loading matrix from: " << matrix_file << std::endl;
        std::ifstream input(matrix_file);
        input.exceptions(std::ifstream::badbit); // set badbit to throw exceptions

        try {
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
        } catch (const std::ifstream::failure& e) {
            std::cerr << "Error while loading matrix: " << e.what() << std::endl;
            throw;
        }
    }

    void LoadStrategyConfigs(const std::string& config_dir) {
        std::cout << "\n" << "Loading strategy configurations from: " << config_dir << std::endl;   
        
        try {
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
            std::cout << "Strategy configurations loaded successfully" << "\n" << std::endl;
        } catch (const std::ifstream::failure& e) {
            std::cerr << "Error while loading strategy configurations: " << e.what() << std::endl;
            throw;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            throw;
        }
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
            scores[i] += matrix[index][i];
        }
    }

    void UpdateHistories(const std::vector<char>& moves) {
        for (int i = 0; i < 3; ++i) {
            histories[i].push_back(moves[i]);
        }
    }

    void PrintStepResults(int step, const std::vector<char>& moves) {
        std::cout << "Step " << step + 1 << ":\n";
        for (int i = 0; i < static_cast<int>(strategies.size()); ++i) {
            std::cout << i + 1 << ". "<< strategies[i]->GetName() << " chose " << moves[i] << "\n";
        }
        std::cout << "Scores: ";
        for (int i = 0; i < static_cast<int>(strategies.size()); ++i) {
            std::cout << scores[i] << " ";
        }
        std::cout << "\n\n";
    }

    void PrintFinalResults() {
        std::cout << "Final results:" << std::endl;
        for (int i = 0; i < static_cast<int>(strategies.size()); ++i) {
            if (tournament_scores.empty()) {
                std::cout << i + 1 << ". " << strategies[i]->GetName() << ": " << scores[i] << std::endl;
            } else {
                std::cout << i + 1 << ". " << strategies[i]->GetName() << ": " << tournament_scores.at({strategies[i]->GetName(), i}) << std::endl;
            }
        }
        std::cout << std::endl;

        // equal scores check
        int max_score = (tournament_scores.empty()) ? *std::max_element(scores.begin(), scores.end()) 
        : std::max_element(tournament_scores.begin(), tournament_scores.end(), 
        [](const auto& a, const auto& b) { return a.second < b.second; })->second;

        std::vector<std::pair<std::string, int>> winners;
        for (int i = 0; i < static_cast<int>(strategies.size()); ++i) {
            if ((tournament_scores.empty() && scores[i] == max_score) ||
                (!tournament_scores.empty() && tournament_scores.at({strategies[i]->GetName(), i}) == max_score)) {
                winners.push_back(std::make_pair(strategies[i]->GetName(), i));
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
};