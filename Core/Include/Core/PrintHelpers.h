#pragma once

#include <format>
#include <iostream>

namespace PrintHelpers
{
// ANSI color codes for terminal output
namespace Color
{
constexpr const char* Reset = "\033[0m";
constexpr const char* Red = "\033[31m";
constexpr const char* Green = "\033[32m";
constexpr const char* Yellow = "\033[33m";
constexpr const char* Blue = "\033[34m";
constexpr const char* Magenta = "\033[35m";
constexpr const char* Cyan = "\033[36m";
constexpr const char* Gray = "\033[90m";
constexpr const char* BrightRed = "\033[91m";
} // namespace Color

// Basic output functions
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

// Leveled logging functions with conventional prefixes
template<typename... Args>
void Info(std::format_string<Args...> fmt, Args&&... args)
{
	std::cout << std::format("{}[INFO]{} {}\n", Color::Cyan, Color::Reset, std::format(fmt, std::forward<Args>(args)...));
}

template<typename... Args>
void Success(std::format_string<Args...> fmt, Args&&... args)
{
	std::cout << std::format(
		"{}[SUCCESS]{} {}\n", Color::Green, Color::Reset, std::format(fmt, std::forward<Args>(args)...));
}

template<typename... Args>
void Warning(std::format_string<Args...> fmt, Args&&... args)
{
	std::cerr << std::format(
		"{}[WARNING]{} {}\n", Color::Yellow, Color::Reset, std::format(fmt, std::forward<Args>(args)...));
}

template<typename... Args>
void Error(std::format_string<Args...> fmt, Args&&... args)
{
	std::cerr << std::format(
		"{}[ERROR]{} {}\n", Color::BrightRed, Color::Reset, std::format(fmt, std::forward<Args>(args)...));
}

template<typename... Args>
void Debug(std::format_string<Args...> fmt, Args&&... args)
{
#ifdef NDEBUG
	// Debug messages are compiled out in release builds
	(void)sizeof...(args);
#else
	std::cout << std::format("{}[DEBUG]{} {}\n", Color::Gray, Color::Reset, std::format(fmt, std::forward<Args>(args)...));
#endif
}

template<typename... Args>
void Trace(std::format_string<Args...> fmt, Args&&... args)
{
#ifdef NDEBUG
	(void)sizeof...(args);
#else
	std::cout << std::format(
		"{}[TRACE]{} {}\n", Color::Magenta, Color::Reset, std::format(fmt, std::forward<Args>(args)...));
#endif
}
} // namespace PrintHelpers
