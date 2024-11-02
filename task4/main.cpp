#include <iostream>
#include <fstream>

#include "TuplePrinter.h"
#include "CSVParser.h"

int main() {
    std::ifstream file("weekday.csv");

    CSVParser<std::string, std::string, std::string, std::string> parser(file, ',', '\n', '"', 0);

    std::cout << "--------------------CSVParser--------------------" << std::endl;

    for (const auto& row : parser) {
        std::cout << row << std::endl;
    }
    
    return 0;
}
