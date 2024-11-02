#include <iostream>
#include <fstream>

#include "TuplePrinter.h"
#include "CSVParser.h"

int main() {
    std::ifstream file("weekday.csv");

    CSVParser parser(file, ',', '\n', '"', 0);

    for (const auto& row : parser) {
        if (row.empty()) continue;
        
        std::cout << "(";
        for (size_t i = 0; i < row.size(); ++i) {
            std::cout << (i == 0 ? "" : ", ") << row[i];
        }
        std::cout << ")" << std::endl;
    }
    
    return 0;
}
