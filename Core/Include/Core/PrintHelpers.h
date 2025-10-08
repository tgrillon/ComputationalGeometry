#pragma once

#include <format>
#include <iostream>

namespace PrintHelpers
{
template<typename... Args>
void print(std::format_string<Args...> fmt, Args&&... args)
{
	std::cout << std::format(fmt, std::forward<Args>(args)...) << '\n';
}

template<typename... Args>
void message(Args&&... args)
{
	print("-- [STATUS] {} --", std::forward<Args>(args)...);
}

template<typename... Args>
void status(Args&&... args)
{
	print("-- [STATUS] {} --", std::forward<Args>(args)...);
}

template<typename... Args>
void info(Args&&... args)
{
	print("## [INFO]   {} ##", std::forward<Args>(args)...);
}

template<typename... Args>
void debug(Args&&... args)
{
	print("## [DEBUG]  {} ##", std::forward<Args>(args)...);
}

template<typename... Args>
void error(Args&&... args)
{
	print("!! [ERROR]  {} !!", std::forward<Args>(args)...);
}
} // namespace PrintHelpers
