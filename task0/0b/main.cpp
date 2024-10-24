#include <iostream>
#include <fstream>

#include <string>
#include <vector> // for dynamic arrays
#include <map>
#include <list>
#include <algorithm> // for sorting
#include <iomanip> // for output formatting

#include <clocale>

// I extract words from line in this function and return a vector of words
std::vector<std::string> ExtractWords(std::string& line) {
    std::string russian_letters = "абвгдежзийклмнопрстуфхцчшщъыьэюя";
    std::vector<std::string> words;
    std::string word; // element of vector, later will be added to vector
    for (char c: line) {
        if (isalnum(c) || russian_letters.find(tolower(c)) != std::string::npos) {
            word += tolower(c);
        } else {
            if (!word.empty()) { // if separator found, add word to vector
                words.push_back(word); // 106 - вектор в качестве стека
                // https://ravesli.com/urok-106-emkost-vektora-std-vector-v-kachestve-steka/#toc-2
                word.clear();
            }
        }
    }
    if (!word.empty()) { // in case the last word is not empty
        words.push_back(word);
    }
    return words;
}

// 1 open input file 
// 2 read line by line and extract words
// 3 increment word frequency in the map
// 4 copy map to a list and sort it by frequency in descending order
// 5 print the list to the output csv file

// int argc - количество аргументов командной строки, где 0 - имя программы
// char* argv[] - массив с указателями на строки с аргументами командной строки

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "ru_RU.cp1251");
    if (argc != 3) { // check number of arguments
        std::cerr << "Error: Wrong number of arguments" << std::endl;
        return 0;
    }

    // std::ifstream in - поток для чтения файла
    std::ifstream input(argv[1]);
    if (!input.is_open()) {
        std::cerr << "Error: Cannot open input file" << std::endl;
        return 0;
    }

    std::map<std::string, int> word_freq; // map to store words and its' frequencies
    std::string line;
    int word_counter = 0;
    while (std::getline(input, line)) {
        std::vector<std::string> words = ExtractWords(line); // extract all words from one line
        for (std::string &word : words) { // increment word frequency in the map
            word_counter++;
            word_freq[word]++;
        }
    }
    input.close();

    std::cout << word_freq.size() << " unique words found" << std::endl;
    std::cout << word_counter << " words found in total" << std::endl;

    // creare list with pairs - elements from map and sort them in descending order
    std::list<std::pair<std::string, int>> sorted_words(word_freq.begin(), word_freq.end());
    auto DescendSort = [](std::pair<std::string, int>& a, std::pair<std::string, int>& b) {
        return a.second > b.second;
    };
    sorted_words.sort(DescendSort); // sort with lambda function DescendSort

    // std::ofstream out - поток для записи (выходной поток) в файл
    std::ofstream output(argv[2]);
    if (!output.is_open()) {
        std::cerr << "Error: Cannot open output file" << std::endl;
        return 0;
    }

    output << std::setprecision(3) << "Word,Frequency,Frequency(%)\n"; // header to the CSV file
    for (const auto &pair : sorted_words) { // pair - это константная ссылка на текущий элемент массива в итерации(91 урок)
        double percentage = static_cast<double>(pair.second) / word_counter * 100;
        output << pair.first << "," << pair.second << "," << percentage << "%\n";
    }
    output.close();

    std::cout << "Success" << std::endl;

    return 0;
}

// how to compile:
// g++ -std=c++11 -o word_count main.cpp
// ./word_count input.txt output.csv