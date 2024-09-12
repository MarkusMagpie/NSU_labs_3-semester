#include "module1.h"
#include "module2.h"
#include "module3.h"
#include <iostream>


int main(int argc, char** argv)
{
	std::cout <<  "Hello world!" << "\n";
	
	std::cout << Module1::getMyName() << "\n";
	std::cout << Module2::getMyName() << "\n";

	using namespace Module1;
	std::cout << getMyName() << "\n"; // (A)
	std::cout << Module2::getMyName() << "\n";

	// using namespace Module2; // (B)
	// std::cout << Module2::getMyName() << "\n"; // COMPILATION ERROR (C)

	using Module2::getMyName;
	std::cout << getMyName() << "\n"; // (D)

    // Указываем компилятору искать идентификатор в конкретном namespace
    std::cout << Module3::getMyName() << "\n";

    return 0;
}

// 1
// при компиляции создается файл a.out, который содержит компилированный код

// 3 
// https://ravesli.com/urok-54-using-statements/#toc-0
// в строке А мы используем using-директиву, т.е. сообщаем компилятору
// что мы используем все объекты из пространства имен Module1.
// в строке D используем using-объявление, т.е. сообщаем компилятору
// что мы используем объект getMyName из пространства имен Module2.

// 4
// при компилировании строки C, получаем ошибку: error: call of overloaded 'getMyName' is ambiguous
// Имеем несколько функций (2) с одинаковым названием: getMyName. 
// Для решения проблемы "двусмысленности", конкретизируем из какого пр-ва имен берем функцию:
    // std::cout << Module2::getMyName() << "\n";
// Возвращенная компилятором ошибка: 
// main.cpp: In function ‘int main(int, char**)’:
// main.cpp:19:31: error: call of overloaded ‘getMyName()’ is ambiguous
//    19 |         std::cout << getMyName() << "\n"; // COMPILATION ERROR (C)
//       |                      ~~~~~~~~~^~
// In file included from main.cpp:2:
// module2.h:5:21: note: candidate: ‘std::string Module2::getMyName()’
//     5 |         std::string getMyName();
//       |                     ^~~~~~~~~
// In file included from main.cpp:1:
// module1.h:5:21: note: candidate: ‘std::string Module1::getMyName()’
//     5 |         std::string getMyName();
//       |                     ^~~~~~~~~

// 5
// Могли во всех header файлах объявить один namespace, в каждом файле написать свою функцию.
// Вероятно речь идет о вложенных пр-вах имен. Тогда в "module3.h" можно написать:
// namespace Module3
// {
//     namespace Module4
//     {
//         std::string getMyName();
//     }
// }
// обращение к функции getMyName: Module3::Module4::getMyName()

// 6
// Используя using-директиву. 
// Пример:
// #include <iostream>


// int main()
// {
//    using namespace std; // сообщаем компилятору, что мы используем все объекты из namespace std
//    cout << "Hello, world!"; // префикса std:: здесь уже не нужно (:: - оператор разрешения области видимости)
//    return 0;
// }