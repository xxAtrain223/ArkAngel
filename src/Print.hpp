//
// Created by Anthony on 5/8/2016.
//

#ifndef ARKANGEL_PRINT_HPP
#define ARKANGEL_PRINT_HPP

#include <print.hpp>

template <typename... Ts>
void Print(const char* format, Ts&&... params)
{
    _PrintDetail<decltype(std::cout)> printer(std::cout);
    printer._print(format, std::forward<Ts>(params)...);
}

template <typename... Ts>
void PrintLog(const char* format, Ts&&... params)
{
    _PrintDetail<decltype(std::clog)> printer(std::clog);
    printer._print(format, std::forward<Ts>(params)...);
}

template <typename... Ts>
void PrintErr(const char* format, Ts&&... params)
{
    _PrintDetail<decltype(std::cerr)> printer(std::cerr);
    printer._print(format, std::forward<Ts>(params)...);
}

template <typename... Ts>
std::string PrintStr(const char* format, Ts&&... params)
{
    std::stringstream out;
    _PrintDetail<std::stringstream> printer(out);
    printer._print(format, std::forward<Ts>(params)...);
    return out.str();
}

#endif //ARKANGEL_PRINT_HPP
