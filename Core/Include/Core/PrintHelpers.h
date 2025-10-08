#pragma once

#include <format>
#include <iostream>

namespace PrintHelpers
{
template<typename... Args>
void Print(std::format_string<Args...> fmt, Args&&... args)
{
	std::cout << std::format(fmt, std::forward<Args>(args)...) << '\n';
}

template<typename... Args>
void PrintErr(std::format_string<Args...> fmt, Args&&... args)
{
	std::cerr << std::format(fmt, std::forward<Args>(args)...) << '\n';
}

template<typename... Args>
void Message(Args&&... args)
{
	print("-- [STATUS] {} --", std::forward<Args>(args)...);
}

template<typename... Args>
void Status(Args&&... args)
{
	Print("-- [STATUS] {} --", std::forward<Args>(args)...);
}

template<typename... Args>
void Info(Args&&... args)
{
	Print("## [INFO]   {} ##", std::forward<Args>(args)...);
}

template<typename... Args>
void Debug(Args&&... args)
{
	Print("## [DEBUG]  {} ##", std::forward<Args>(args)...);
}

template<typename... Args>
void Error(Args&&... args)
{
	PrintErr("!! [ERROR]  {} !!", std::forward<Args>(args)...);
}
} // namespace PrintHelpers
