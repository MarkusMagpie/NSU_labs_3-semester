#include <iostream>
#include <string>

#include "Game.h"

int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::string file = "input.txt";
        if (argv[1] != nullptr) {
            file = argv[1];
        }
        Game game(file);

        if (argc > 3 && (std::string(argv[2]) == "-i" || std::string(argv[2]) == "--iterations")) {
            int iterations = std::stoi(argv[3]);
            std::string output_file = (argc > 5 && (std::string(argv[4]) == "-o" || std::string(argv[4]) == "--output")) 
            ? argv[5]
            : "output.txt";

            std::cout << "-----OFFLINE MODE-----" << std::endl;
            game.OfflineMode(iterations, output_file);
        } else {
            game.Run();
        }
    } else {
        Game game("");
        game.Run();
    }

    return 0;
}