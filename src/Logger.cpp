#include "Logger.h"
#include <cstdlib>
#ifdef _WIN32
	#include <windows.h>
#elif defined(__linux__) && !defined(__ANDROID__)
	#include <unistd.h>
#endif
static bool supportsColor() {
	#ifdef _WIN32
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hOut == INVALID_HANDLE_VALUE) return false;
		DWORD dwMode = 0;
		if (!GetConsoleMode(hOut, &dwMode)) return false;
		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		if (!SetConsoleMode(hOut, dwMode)) return false;
		return true;
	#elif defined(__linux__) && !defined(__ANDROID__)
		if (!isatty(STDOUT_FILENO)) return false;
		const char* term = std::getenv("TERM");
		if (term != nullptr) {
			if (std::string(term) == "dumb") return false;
			return true;
		}
	#else
		return false;
	#endif
}
void Logger::print(Priority p, const std::string& msg) {
	#ifdef __ANDROID__
		switch (p) {
			case Priority::Info:
				LOGI("%s", msg.c_str());
				break;
			case Priority::Error:
				LOGE("%s", msg.c_str());
				break;
		}
	#else
		static bool hasColor = supportsColor();
		switch (p) {
			case Priority::Info:
				if (hasColor) std::cout << "\033[32m" << msg << "\033[0m" << std::endl;
				else std::cout << "[INFO] " << msg << std::endl;
				break;
			case Priority::Error:
				if (hasColor) std::cerr << "\033[31m" << msg << "\033[0m" << std::endl;
				else std::cerr << "[ERROR] " << msg << std::endl;
				break;
		}
	#endif
}