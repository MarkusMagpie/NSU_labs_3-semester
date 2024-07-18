#pragma once

#include "FileHandler.h"

#include <map>
#include <string>
#include <functional>
#include <sstream>

class CommandHandler {
private: 
    Universe& universe;
    std::map<std::string, std::function<void(const std::string&)>> commands;

    void Dump(const std::string& cmds);
    void Tick(const std::string& cmds);
    void Exit(const std::string& cmds);
    void Help(const std::string& cmds); 
public:
    CommandHandler(Universe& universe);
    void HandleCommand(const std::string& command);
};

// связываем строковые команды с лямбда-функциями
CommandHandler::CommandHandler(Universe& universe) : universe(universe) {
    // std::cout << "Commands: dump, tick, t, exit, help" << std::endl;
    commands["dump"] = [this](const std::string& cmds) { Dump(cmds); };
    commands["tick"] = [this](const std::string& cmds) { Tick(cmds); };
    commands["t"] = [this](const std::string& cmds) { Tick(cmds); };
    commands["exit"] = [this](const std::string& cmds) { Exit(cmds); };
    commands["q"] = [this](const std::string& cmds) { Exit(cmds); };
    commands["help"] = [this](const std::string& cmds) { Help(cmds); };
}

void CommandHandler::HandleCommand(const std::string& command) {
    std::istringstream input(command);
    std::string cmd;
    std::string cmds;
    input >> cmd; // read first word of command. "dump" for example
    
    std::getline(input, cmds); // read rest of command. " filename.txt "

    auto it = commands.find(cmd);
    if (it != commands.end()) {
        it->second(cmds); // call lambda function of found command key
        // example: [this](const std::string& cmds) { Dump(cmds); }
    } else {
        std::cout << "Unknown command: " << cmd << std::endl;
    }
}

// пример: dump filename.txt
// cmds = " filename.txt " 
void CommandHandler::Dump(const std::string& cmds) {
    std::string output = cmds.substr(1);
    if (output.empty()) {
        output = "output.txt";
    }
    FileHandler file_handler;
    file_handler.SaveToFile(universe, output);
    std::cout << "Universe saved to: " << output << std::endl;
}

void CommandHandler::Tick(const std::string& cmds) {
    int iterations = 1;
    if (!cmds.empty()) {
        iterations = std::stoi(cmds.substr(1));   
    }
    std::cout << "\n-----ITERATION: " << universe.GetCurrentIteration() + iterations << "-----" << std::endl;
    universe.tick(iterations);
    universe.print();
}

void CommandHandler::Exit(const std::string& cmds) {
    std::cout << "Goodbye!" << std::endl;
    std::exit(0);
}

void CommandHandler::Help(const std::string& cmds) {
    std::cout << "Available commands:\n";
    std::cout << "dump <filename> - Save the universe to a file\n";
    std::cout << "tick <n=1> or t <n=1> - Calculate n iterations and print the result\n";
    std::cout << "exit - Exit the game\n";
    std::cout << "help - Print this help message\n";
}