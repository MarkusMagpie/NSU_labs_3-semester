#include <iostream>
#include <string>

#include "Game.h"

std::map<std::string, std::string> ParseCommandLine(int argc, char* argv[]) {
    std::map<std::string, std::string> args;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.find('=') != std::string::npos) {
            std::string::size_type pos = arg.find('=');
            std::string key, value;
            key = arg.substr(0, pos);
            value = arg.substr(pos + 1);
            
            if (key == "-i" || key == "--iterations") {
                args["iterations"] = value;
            } else if (key == "-o" || key == "--output") {
                args["output"] = value;
            } else {
                args["input"] = key; // will be only key, no value
            }
        } else {
            if (arg == "-i" || arg == "--iterations") {
                args["iterations"] = argv[i + 1];
                ++i;
            } else if (arg == "-o" || arg == "--output") {
                args["output"] = argv[i + 1];
                ++i;
            } else {
                args["input"] = arg;
            }
        }
    }

    // std::cout << "input: " << args["input"] << "\n";
    // std::cout << "iterations: " << args["iterations"] << "\n";
    // std::cout << "output: " << args["output"] << std::endl;

    return args;
}

int main(int argc, char* argv[]) {
    // if (argc > 1) {
    //     std::string file = "input.txt";
    //     if (argv[1] != nullptr) {
    //         file = argv[1];
    //     }
    //     Game game(file);

    //     if (argc > 3 && (std::string(argv[2]) == "-i" || std::string(argv[2]) == "--iterations")) {
    //         int iterations = std::stoi(argv[3]);
    //         std::string output_file = (argc > 5 && (std::string(argv[4]) == "-o" || std::string(argv[4]) == "--output")) 
    //         ? argv[5]
    //         : "output.txt";

    //         // std::cout << "argv[1] = " << file << std::endl;
    //         // std::cout << "argv[2] = " << argv[2] << std::endl;
    //         // std::cout << "argv[3] = " << argv[3] << std::endl;
    //         // std::cout << "argv[4] = " << argv[4] << std::endl;
    //         // std::cout << "argv[5] = " << argv[5] << std::endl;

    //         std::cout << "-----OFFLINE MODE-----" << std::endl;
    //         game.OfflineMode(iterations, output_file);
    //     } else {

    //         // std::cout << "argv[1] = " << file << std::endl;
    //         // std::cout << "argv[2] = " << argv[2] << std::endl;
    //         // std::cout << "argv[3] = " << argv[3] << std::endl;
    //         // std::cout << "argv[4] = " << argv[4] << std::endl;

    //         game.Run();
    //     }
    // } else {
    //     Game game("");
    //     game.Run();
    // }

    auto args = ParseCommandLine(argc, argv);
    std::string file = args.count("input") ? args["input"] : "input.txt";
    if (file == "") {
        std::cout << "No input file provided, using default input.txt" << std::endl;
        file = "input.txt";
    }
    Game game(file);

    if (args.count("iterations")) {
        int iterations = std::stoi(args["iterations"]);
        std::string output_file = args.count("output") ? args["output"] : "output.txt";
        if (output_file == "") {
            std::cout << "No output file provided, using default output.txt" << std::endl;
            output_file = "output.txt";
        }

        std::cout << "-----OFFLINE MODE-----" << std::endl;
        game.OfflineMode(iterations, output_file);
    } else {
        std::cout << "-----ONLINE MODE-----" << std::endl;
        game.Run();
    }

    return 0;
}