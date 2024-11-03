#include <iostream>
#include <fstream>

#include "TuplePrinter.h"
#include "CSVParser.h"

int main() {
    std::cout << "SUBTASK 1 TEST" << std::endl;
    std::tuple<std::string, std::string, std::string, std::string> tuple{"Mon", "Tue", "Wed", "Thu"};
    std::cout << tuple << "\n" << std::endl;

    std::cout << "SUBTASK 2 AND 3 TEST" << std::endl;
    std::ifstream file("weekday.csv");
    CSVParser<std::string, std::string, std::string, std::string> parser(file, ',', '\n', '"', 0);
    for (const auto& row : parser) {
        // std::cout << "iteration" << std::endl;
        std::cout << row << std::endl;
    }
    
    return 0;
}
