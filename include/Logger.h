#include <iostream>
#include <string>
#define LOG_TAG "DEngine"
#ifdef __ANDROID__
	#include <android/log.h>
	#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%s", __VA_ARGS__)
	#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "%s", __VA_ARGS__)
#endif
class Logger {
public:
	enum Priority { Error, Info };
	static void print(Priority p, std::string msg);
	Logger() = default;
};