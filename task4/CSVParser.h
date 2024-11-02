#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <tuple>
#include <string>
#include <vector>

class CSVParser {
private:
    std::ifstream& file;
    std::vector<std::string> current_row;
    bool eof = false;

    char column_delimeter;
    char row_delimeter;
    char escape_character;
public:
    CSVParser(std::ifstream& file, char column_delim = ',', char row_delim = '\n', char escape_char = '"', int skip_lines = 0) 
    : file(file), column_delimeter(column_delim), row_delimeter(row_delim), escape_character(escape_char)
    {
        for (int i = 0; i < skip_lines; ++i) { // пропуск строк до начальной 
            std::string dummy;
            std::getline(file, dummy);
        }
    }

    // итератор возвращает сам себя
    CSVParser& begin() {
        return *this;
    }

    CSVParser& end() {
        return *this;
    }

    // сравнение для окончания итерации
    bool operator!=(const CSVParser&) const { return !eof; }

    // оператор разыменования - возвращает текущую строку
    const std::vector<std::string>& operator*() const {
        return current_row;
    }

    // перегруженный оператор++ - оператор инкремента (переход к следующей строке)
    CSVParser& operator++() {
        std::string line;
        current_row.clear(); // очистка текущей строки для нового чтения
        if (std::getline(file, line, row_delimeter)) {
            // std::cout << "Reading line: " << line << std::endl;
            parse_line(line, current_row); // разбор строки в вектор
        } else {
            eof = true;   
        }

        return *this;
    }

    // Функция для разбора строки в вектор строк
    void parse_line(const std::string& line, std::vector<std::string>& row) {
        std::stringstream line_stream(line); // поток для обработки строки
        std::string value; // текущее значение

        while (std::getline(line_stream, value, column_delimeter)) {
            // std::cout << "Value: " << value << std::endl;

            if (!value.empty() && value.front() == escape_character && value.back() == escape_character) {
                value = value.substr(1, value.size() - 2);
            }
            
            row.push_back(value);
        }
    }
};
