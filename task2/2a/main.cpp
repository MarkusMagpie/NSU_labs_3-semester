#include "PrisonersDilemmaSimulator.h"
#include "BaseStrategies.h"

#include <map>
#include <string>

std::map<std::string, std::string> ParseCommandLine(int argc, char* argv[]) {
    std::map<std::string, std::string> args;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.substr(0, 2) == "--") {
            std::string::size_type pos = arg.find('=');
            if (pos != std::string::npos) {
                std::string key = arg.substr(2, pos - 2);
                std::string value = arg.substr(pos + 1);
                args[key] = value;
            } else {
                std::string key = arg.substr(2);
                args[key] = "";
            }
        }
    }

    return args;
}

void RegisterBaseStrategies() {
    auto base_strat1 = []() { return std::make_unique<PoorTrustingFool>(); };
    auto base_strat2 = []() { return std::make_unique<AllDefect>(); };
    auto base_strat3 = []() { return std::make_unique<Random>(); };
    auto base_strat4 = []() { return std::make_unique<GoByMajority>(); };
    auto base_strat5 = []() { return std::make_unique<TitForTat>(); };

    StrategyFactory::GetInstance().RegisterStrategy("PoorTrustingFool", base_strat1);
    StrategyFactory::GetInstance().RegisterStrategy("AllDefect", base_strat2);
    StrategyFactory::GetInstance().RegisterStrategy("Random", base_strat3);
    StrategyFactory::GetInstance().RegisterStrategy("GoByMajority", base_strat4);
    StrategyFactory::GetInstance().RegisterStrategy("TitForTat", base_strat5);
}
// used lambda function here
// each lambda has the type std::unique_ptr<Strategy>()
// RegisterStrategy's 2nd argument type: std::function<std::unique_ptr<Strategy>()>  => 
// given I already have std::unique_ptr<Strategy>() it will be implicitly converted to RegisterStrategy
// https://metanit.com/cpp/tutorial/15.4.php

int main(int argc, char* argv[]) {
    RegisterBaseStrategies();

    if (argc < 4) {
        return 0;
    }

    std::vector<std::string> strategies = { argv[1], argv[2], argv[3] };
    
    auto args = ParseCommandLine(argc, argv);

    // simulation mode
    std::string simulation_mode = args["mode"];
    if (simulation_mode.empty()) {
        simulation_mode = (strategies.size() > 3) ? "tournament" : "detailed";
    }

    // optional numer of steps
    int steps = 10;
    if (args.find("steps") != args.end()) { // check if "steps" is in the map
        steps = std::stoi(args["steps"]);
    }

    // optional confiduration directory
    std::string config_dir;
    if (args.find("configs") != args.end()) {
        config_dir = args["configs"];
    }

    // optional matrix file
    std::string matrix_file = "default_matrix.txt";
    if (args.find("matrix") != args.end()) {
        matrix_file = args["matrix"];
    }

    try {
        PrisonersDilemmaSimulator sim(strategies, steps, matrix_file, config_dir);

        if (simulation_mode == "detailed") {
            sim.Run(true);
        } else if (simulation_mode == "fast") {
            sim.Run(false);
        } else if (simulation_mode == "tournament") {
            // TODO
            return 0;
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