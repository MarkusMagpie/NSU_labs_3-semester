#include "main.h"

std::map<std::string, std::string> ParseCommandLine(int argc, char* argv[]) {
    std::map<std::string, std::string> args;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.find("--") == 0) {
            size_t pos = arg.find('=');

            std::string key, value;
            key = arg.substr(2, pos - 2);
            value = (pos != std::string::npos) ? arg.substr(pos + 1) : "";
            args[key] = value;
        }
    }

    return args;
}

int main(int argc, char* argv[]) {
    try {
        CheckArguments(argc, argv);
    } catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
        return 0;
    }

    RegisterBaseStrategies();
    RegisterCustomStrategies();

    std::vector<std::string> strategies;
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] != '-') {
            strategies.push_back(argv[i]);
        }
    }
    
    auto args = ParseCommandLine(argc, argv);

    // simulation mode
    std::string simulation_mode = (strategies.size() > 3) ? "tournament" : "detailed";
    if (args.find("mode") != args.end()) {
        simulation_mode = args["mode"];
    }

    // optional numer of steps
    int steps = 10;
    if (args.find("steps") != args.end()) { // check if "steps" is in the args
        steps = std::stoi(args["steps"]);
        if ((steps <= 0) || (typeid(steps) != typeid(int))) { // check for inappropriate value
            steps = 10;
        }
    }

    // optional confiduration directory
    std::string config_dir;
    if (args.find("configs") != args.end()) {
        config_dir = args["configs"];
    }

    // optional matrix file
    std::string matrix_file;
    if (args.find("matrix") != args.end()) {
        matrix_file = args["matrix"];
    }

    try {
        PrisonersDilemmaSimulator sim(strategies, steps, matrix_file, config_dir);

        if (simulation_mode == "detailed") {
            sim.RunDetailed();
        } else if (simulation_mode == "fast") {
            sim.Run(false);
        } else if (simulation_mode == "long") {
            sim.Run(true);   
        } else if (simulation_mode == "tournament") {
            sim.RunTournament();
        } else {
            std::cerr << "Unknown simulation mode: " << simulation_mode << std::endl;
            return 0;
        }
    } catch (const std::exception& e) {
        std::cout << "Error in main - failed simulator run" << std::endl;
        std::cerr << "Error: " << e.what() << std::endl;
        return 0;
    }

    return 0;
}