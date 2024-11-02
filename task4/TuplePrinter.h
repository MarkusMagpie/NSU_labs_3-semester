#pragma once

#include <iostream>
#include <tuple>

// рекурсивный шаблон для печати кортежа поэлементно
template<typename Tuple, std::size_t... Is>
void print_tuple_impl(std::ostream& os, const Tuple& t, std::index_sequence<Is...>) {
    // каждый элемент кортежа печатаем с помощью std::get и индекса Is
    // например: std::get<0>(t)
    ((os << (Is == 0 ? "" : ", ") << std::get<Is>(t)), ...);
}

// перегрузка оператора вывода для печати tuple
// std::ostream& os - ссылка на поток ввода (например std::cout)
// const std::tuple<Args...>& t - кортеж для печати
template<typename... Args>
std::ostream& operator<<(std::ostream& os, const std::tuple<Args...>& t) {
    os << "(";
    print_tuple_impl(os, t, std::index_sequence_for<Args...>{}); // index_sequence_for - генерация последовательности индексов
    os << ")";
    return os;
}