#include "Logger.h"
void Logger::print(Priority p, std::string msg) {
	switch (p) {
	case Info:
		#ifdef __ANDROID__
			LOGI(msg.c_str());
		#else
			std::cout << msg << std::endl;
		#endif
		break;
	case Error:
		#ifdef __ANDROID__
			LOGE(msg.c_str());
		#else
			std::cout << msg << std::endl;
		#endif
		break;
	default:
		break;
	}
}