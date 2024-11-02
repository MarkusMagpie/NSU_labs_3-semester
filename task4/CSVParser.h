#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <tuple>
#include <string>
#include <vector>

template <typename... Args>
class CSVParser {
private:
    using value_type = std::tuple<Args...>;

    std::ifstream& file;      // ссылка на файл
    value_type current_tuple; // текущий кортеж
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
        ++(*this); // добавил начальную загрузку первой строки (БЫЛА ОШИБКА)
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
    const value_type& operator*() const { return current_tuple; }

    // перегруженный оператор++ - оператор инкремента (переход к следующей строке)
    CSVParser& operator++() {
        std::string line;
        if (std::getline(file, line, row_delimeter)) {
            parse_line(line); // разбор строки в вектор
        } else {
            eof = true;   
        }

        return *this;
    }

    // разбор строки в вектор значений
    std::vector<std::string> split_line(const std::string& line) {
        std::vector<std::string> result;
        std::stringstream line_stream(line);
        std::string value;
        
        while (std::getline(line_stream, value, column_delimeter)) {
            // удаление символов экранирования, если они есть
            if (!value.empty() && value.front() == escape_character && value.back() == escape_character) {
                value = value.substr(1, value.size() - 2);
            }
            result.push_back(value);
        }

        return result;
    }

    // преобразование вектора строк в кортеж значений
    template<std::size_t... Is>
    void fill_tuple(const std::vector<std::string>& values, value_type& tuple, std::index_sequence<Is...>) {
        // Используем std::istringstream для конвертации каждой строки в значение нужного типа
        ((std::istringstream(values[Is]) >> std::get<Is>(tuple)), ...);
    }

    // функция для разбора строки в вектор строк
    void parse_line(const std::string& line) {
        auto values = split_line(line); // разделяем строку на значения

        if (values.size() != sizeof...(Args)) { // проверка количества столбцов
            throw std::runtime_error("Количество значений в строке не совпадает с количеством типов в изначальном кортеже.");
        }
        fill_tuple(values, current_tuple, std::index_sequence_for<Args...>{}); // index_sequence_for - генерация последовательности индексов
    }
};
