#include "BasicExample.h"

#include <iostream>
#include <thread>
#include <iostream>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define WINDOWS
#endif

#if defined(WINDOWS)
#include <windows.h>
#endif

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

using namespace std;

enum class Color {
	red, orange, green
};

string timestamp() {
	time_t now = time(0);
	tm* ltm = localtime(&now);

	char timeString[64];
	sprintf(
		timeString,
		"%02d:%02d:%02d",
		ltm->tm_hour,
		ltm->tm_min,
		ltm->tm_sec);

	return string(timeString);
}

void print(const std::string &message, const Color &color) {
#if defined(WINDOWS)
	WORD attribs = 0;
	switch (color) {
	case Color::green:
		attribs = FOREGROUND_GREEN;
		break;
	case Color::orange:
		attribs = FOREGROUND_RED;
		break;
	case Color::red:
		attribs = FOREGROUND_RED;
		break;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attribs);
#else
  unused(color);
#endif
	std::cout << timestamp() << ": " << message << std::endl;
#if defined(WINDOWS)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
#endif
}

BasicExample::BasicExample(const std::string &name): m_name(name) { 
	// Nop
}

std::string BasicExample::name() const {
	return m_name;
}

void BasicExample::printInfo(const std::string &message) const {
	print(name() + ": " + message, Color::green);
}

void BasicExample::printWarning(const std::string &message) const {
	print(name() + ": " + message, Color::orange);
}

void BasicExample::printError(const std::string &message) const {
	print(name() + ": " + message, Color::red);
}

void BasicExample::waitForInput() const {
	auto message = name() + ": " + "Press any key to continue";
	print(message, Color::green);
	std::cin.get();
}

#ifdef _MSC_VER
#pragma warning(enable:4996)
#endif