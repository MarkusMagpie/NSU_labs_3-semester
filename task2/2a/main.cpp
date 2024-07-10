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
    StrategyFactory::GetInstance().RegisterStrategy("PoorTrustingFool", []() { return std::make_unique<PoorTrustingFool>(); });
    StrategyFactory::GetInstance().RegisterStrategy("AllDefect", []() { return std::make_unique<AllDefect>(); });
}

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
    if (args.find("config") != args.end()) {
        config_dir = args["config"];
    }

    // optional matrix file
    std::string matrix_file = "default_matrix.txt";
    if (args.find("matrix") != args.end()) {
        matrix_file = args["matrix"];
    }

    try {
        PrisonersDilemmaSimulator sim(strategies, steps, matrix_file);

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
        std::cerr << "Error: " << e.what() << std::endl;
        return 0;
    }

    return 0;
}