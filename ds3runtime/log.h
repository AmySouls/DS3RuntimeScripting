#pragma once
#include "pch.h"

namespace ds3runtime {

class Log {
public:
	Log(HANDLE consoleHandle);

	void info(const char* message)
	{
		cStringOut(message, "info", 10);
	}

	void info(const wchar_t* message)
	{
		wcStringOut(message, "info", 10);
	}

	void warn(const char* message)
	{
		cStringOut(message, "warn", 14);
	}

	void warn(const wchar_t* message)
	{
		wcStringOut(message, "warn", 14);
	}

	void error(const char* message)
	{
		cStringOut(message, "error", 12);
	}

	void error(const wchar_t* message)
	{
		wcStringOut(message, "error", 12);
	}
private:
	void metaInfoOut(const char* messageStatus, uint32_t statusColor) {
		SetConsoleTextAttribute(consoleHandle, 7);
		std::cout << "[DS3RuntimeScripting] [";
		SetConsoleTextAttribute(consoleHandle, statusColor);
		std::cout << messageStatus;
		SetConsoleTextAttribute(consoleHandle, 7);
		std::cout << "] ";
		SetConsoleTextAttribute(consoleHandle, 7);
	}

	void cStringOut(const char* message, const char* messageStatus, uint32_t statusColor)
	{
		metaInfoOut(messageStatus, statusColor);
		std::cout << message << '\n';
	}

	void wcStringOut(const wchar_t* message, const char* messageStatus, uint32_t statusColor)
	{
		metaInfoOut(messageStatus, statusColor);
		std::wcout << message << '\n';
	}

	HANDLE consoleHandle;
};

}