#pragma once

#include <iostream>
#include <tuple>

// template <typename Ch, typename Tr>
// std::basic_ostream<Ch, Tr>& print_tuple(std::basic_ostream<Ch, Tr>& os, const std::tuple<>& t) {
//     return os;
// }

// std::basic_ostream<Ch, Tr>& print_tuple(std::basic_ostream<Ch, Tr>& os, const std::tuple<T, Args...>& t) {
//     os << std::get<0>(t); // печать 1 элемента кортежа t
//     if constexpr (sizeof...(Args) > 0) { // есть ли в кортеже t еще элементы?
//         os << ", ";
//         print_tuple(os, std::tuple<Args...>(std::get<Args + 1>(t)...)); // рекурсивный вызов оставшихся элементов
//     }
//     return os;
// }

// template <typename Ch, typename Tr, typename... Args>
// std::basic_ostream<Ch, Tr>& operator<<(std::basic_ostream<Ch, Tr>& os, const std::tuple<Args...>& t) {
//     os << "(";
//     print_tuple(os, t);
//     os << ")";
//     return os;
// }

// рекурсивный шаблон для печати кортежа поэлементно
template<typename Tuple, std::size_t... Is>
void print_tuple_impl(std::ostream& os, const Tuple& t, std::index_sequence<Is...>) {
    // Expanding the parameter pack to print each element
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