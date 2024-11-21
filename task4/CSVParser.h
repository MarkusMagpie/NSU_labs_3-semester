#pragma once

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

    int current_line = 0;           // текущая строка 
    int last_failed_column = -1;    // последняя неудачная колонка 
public:
    CSVParser(std::ifstream& file, char column_delim = ',', char row_delim = '\n', char escape_char = '"', int skip_lines = 0) 
    : file(file), column_delimeter(column_delim), row_delimeter(row_delim), escape_character(escape_char)
    {
        if (skip_lines < 0) {
            throw std::runtime_error("Значение параметра skip_lines выходит за пределы допустимого диапазона.");
        }

        for (int i = 0; i < skip_lines; ++i) { // пропуск строк до начальной 
            std::string dummy;
            std::getline(file, dummy);
            if (file.eof()) {
                eof = true; // => operator!= не выполняется => итерация завершается
                std::cout << "Конец файла достигнут при пропуске " << i + 1 << " строк из " << skip_lines << std::endl;
                break;
            }
        }

        ++(*this); // добавил начальную загрузку первой строки (БЫЛА ОШИБКА)
        // начальное чтение первой строки с использованием перегруженного оператора ++
    }

    // итератор возвращает сам себя
    CSVParser& begin() {
        // std::cout << "begin" << std::endl;
        return *this;
    }

    CSVParser& end() {
        // std::cout << "end" << std::endl;
        return *this;
    }

    // сравнение для окончания итерации
    bool operator!=(const CSVParser&) const { 
        // std::cout << "operator!=" << std::endl;
        return !eof; 
    }

    // оператор разыменования - возвращает текущую строку
    const value_type& operator*() const { 
        // std::cout << "operator*" << std::endl;
        return current_tuple; 
    }

    // перегруженный оператор++ - переход к следующей строке с ее разбором в current_tuple
    CSVParser& operator++() {
        // std::cout << "operator++" << std::endl;
        std::string line;
        if (std::getline(file, line, row_delimeter)) {
            ++current_line;
            ParseLine(line); // разбор строки, сохранение в current_tuple
        } else {
            eof = true;   
        }

        return *this; // возвращает current_tuple т.к. перегрузка *
    }

    // разбор строки в вектор значений
    std::vector<std::string> SplitLine(const std::string& line) {
        std::vector<std::string> result;
        std::stringstream line_stream(line);
        std::string value;
        
        // считываем значения из строки пока не встретим разделитель column_delimeter
        // https://en.cppreference.com/w/cpp/string/basic_string/getline
        while (std::getline(line_stream, value, column_delimeter)) { 
            // удаление символов экранирования, если они есть ("dfefg", " - escape character)
            if (!value.empty() && value.front() == escape_character && value.back() == escape_character) {
                std::cout << "Нашел символ экранирования в строке: " << value << ", удаляю его" << std::endl;
                value = value.substr(1, value.size() - 2);
            }

            result.push_back(value);
        }

        return result;
    }

    // преобразование вектора строк в кортеж значений
    
    // Пройти по каждому индексу Is из параметр-пакета Is...
    // Выполнить одно и то же выражение (считывание и запись в элемент кортежа current_tuple) для каждого индекса.
    template<std::size_t... Is>
    void CheckFillTuple(const std::vector<std::string>& values, value_type& tuple, std::index_sequence<Is...>) {
        bool success = true;
        // Проходим по каждому элементу values и преобразуем его в соответствующий тип.
        // Если встречаем ошибку, то фиксируем и выводим информацию о позиции ошибкт.
        ((success &= FillTuple(values[Is], std::get<Is>(tuple), Is + 1)), ...);
        
        if (!success) {
            eof = true;
            return;
        }
        // ((std::istringstream(values[Is]) >> std::get<Is>(tuple)), ...);
    }

    // пытается преобразовать значение из строки в требуемый тип
    template <typename T>
    bool FillTuple(const std::string& value_str, T& value, int column) {
        std::istringstream iss(value_str);
        if (!(iss >> value)) {
            last_failed_column = column; // отслеживаем неудачную колонку
            std::cout << "Ошибка типа данных в строке " << current_line 
                      << ", колонке " << column << ": " << value_str << std::endl;
            
            return false;
        }

        return true;
    }

    // функция для разбора строки в вектор строк
    void ParseLine(const std::string& line) {
        auto values = SplitLine(line); // разделяем строку на значения

        if (values.size() != sizeof...(Args)) { // проверка количества столбцов
            throw std::runtime_error("Количество значений в строке " + std::to_string(current_line) + 
                                     " не совпадает с количеством типов в кортеже.");
        }
        CheckFillTuple(values, current_tuple, std::index_sequence_for<Args...>{}); // index_sequence_for - генерация последовательности индексов
    }
};
