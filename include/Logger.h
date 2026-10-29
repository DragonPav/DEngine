#pragma once
#include <iostream>
#include <string>
#define LOG_TAG "DEngine"
#ifdef __ANDROID__
	#include <android/log.h>
	#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
	#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif
/** @class Logger
 * @brief Used for logs (recommended)
 */
class Logger {
public:
	enum Priority { Error, Info };
	/** @brief Print log to OS logging system */
	static void print(Priority p, const std::string& msg);
	Logger() = default;
};