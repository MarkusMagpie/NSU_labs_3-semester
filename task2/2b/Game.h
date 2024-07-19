#pragma once

#include "CommandHandler.h"

class Game {
private: 
    Universe universe;
    CommandHandler* command_handler; // pointer to CommandHandler object

    void InitializeFromFile(const std::string& file);
    void InitializeRandom();
public:
    Game(const std::string& file);
    ~Game() { delete command_handler; }
    void Run();
    void OfflineMode(int iterations, const std::string& output_file);
};

Game::Game(const std::string& file) 
    : universe(0, 0, std::vector<std::vector<bool>>(20, std::vector<bool>(20, false))) // temporary
{
    // if (!file.empty()) {
    //     InitializeFromFile(file);
    // } else {
    //     InitializeRandom();
    // }

    InitializeFromFile(file);

    command_handler = new CommandHandler(universe);
}

void Game::Run() {
    // assistance for new users
    std::cout << "Automatically called help() to assist new users" << std::endl;
    command_handler->HandleCommand("help");

    std::string command;
    while (true) {
        std::cout << "Enter command: ";
        std::getline(std::cin, command);
        command_handler->HandleCommand(command);
    }
}

void Game::OfflineMode(int iterations, const std::string& output_file) {
    universe.tick(iterations);
    FileHandler file_handler;
    file_handler.SaveToFile(universe, output_file);
    std::cout << "Universe saved to: " << output_file << " after " << iterations << " iterations" <<std::endl;
}

// void Game::InitializeRandom() {
//     int width = 10;
//     int height = 10;
//     std::vector<std::vector<bool>> initial_state(height, std::vector<bool>(width, false));

//     // Gosper Glider Gun
//     //square
//     initial_state[5][1] = initial_state[5][2] = initial_state[6][1] = initial_state[6][2] = true;
//     // object 2
//     initial_state[5][11] = initial_state[6][11] = initial_state[7][11] = true;
//     initial_state[4][12] = initial_state[8][12] = true;
//     initial_state[3][13] = initial_state[9][13] = true;
//     initial_state[3][14] = initial_state[9][14] = true;
//     initial_state[6][15] = true;
//     initial_state[4][16] = initial_state[8][16] = true;
//     initial_state[5][17] = initial_state[6][17] = initial_state[7][17] = true;
//     initial_state[6][18] = true;
//     // object 3
//     initial_state[3][21] = initial_state[4][21] = initial_state[5][21] = true;
//     initial_state[3][22] = initial_state[4][22] = initial_state[5][22] = true;
//     initial_state[2][23] = initial_state[6][23] = true;
//     initial_state[1][25] = initial_state[2][25] = initial_state[6][25] = initial_state[7][25] = true;
//     // square
//     initial_state[3][35] = initial_state[4][35] = initial_state[3][36] = initial_state[4][36] = true;

//     universe = Universe(width, height, initial_state);
//     universe.SetName("Random Initialization");
//     universe.SetRule("B3/S23");
// }

void Game::InitializeFromFile(const std::string& file) {
    FileHandler file_handler;
    universe = file_handler.LoadFromFile(file);
    std::cout << "Universe: " << universe.GetName() << std::endl;
    std::cout << "Rule: " << universe.GetRule() << std::endl;
    universe.print();
    std::cout << std::endl;
}