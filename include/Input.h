#pragma once
#include "RenderUtils.h"
#ifdef _WIN32
	#include <GLFW/glfw3.h>
#else
	#include <android/input.h>
#endif
#include <unordered_map>
#include <string>
#include <functional>
class Input {
private:
	std::unordered_map<int, std::function<void(int, int)>> mouseButtonCallbacks;
	std::unordered_map<int, std::function<void(double, double)>> mousePosCallbacks;
	std::unordered_map<int, std::function<void(int, int)>> keyboardCallbacks;
	int nextMouseButtonId = 0;
	int nextMousePosId = 0;
	int nextKeyboardId = 0;
	#ifdef _WIN32
		void mouse_button(GLFWwindow* win, int button, int action, int mods);
		void mouse_move(GLFWwindow* win, double xpos, double ypos);
		void key_callback(GLFWwindow* win, int key, int action, int mods);
		GLFWwindow* window = nullptr;
	#else
		void* window = nullptr;
	#endif
public:
	static const int KEY_PRESSED = 1;
	static const int KEY_RELEASED = 0;
	int addMouseButtonCallback(std::function<void(int, int)> cb);
	void removeMouseButtonCallback(int id);
	int addMousePosCallback(std::function<void(double, double)> cb);
	void removeMousePosCallback(int id);
	int addKeyboardCallback(std::function<void(int, int)> cb);
	void removeKeyboardCallback(int id);
	Input() = delete;
	#ifdef _WIN32
		Input(GLFWwindow* win);
	#else
		bool handleAndroidInput(AInputEvent* event);
		Input(void* win);
	#endif
	~Input();
};