#include "Input.h"
static Input* input;
#ifdef _WIN32
	void Input::mouse_button(GLFWwindow* win, int button, int action, int mods) {
		for (std::pair<int, std::function<void(int, int)>> pair : mouseButtonCallbacks) {
			pair.second(button, action);
		}
	}
	void Input::mouse_move(GLFWwindow* win, double xpos, double ypos) {
		for (std::pair<int, std::function<void(double, double)>> pair : mousePosCallbacks) {
			pair.second(xpos, ypos);
		}
	}
	void Input::key_callback(GLFWwindow* win, int key, int action, int mods) {
		for (std::pair<int, std::function<void(int, int)>> pair : keyboardCallbacks) {
			pair.second(key, action);
		}
	}
#else
	bool Input::handleAndroidInput(AInputEvent* event) {
		int32_t type = AInputEvent_getType(event);
		if (type == AINPUT_EVENT_TYPE_MOTION) {
			int32_t action = AMotionEvent_getAction(event);
			int32_t actionMasked = action & AMOTION_EVENT_ACTION_MASK;

			double x = AMotionEvent_getX(event, 0);
			double y = AMotionEvent_getY(event, 0);
			for (std::pair<int, std::function<void(double, double)>> pair : mousePosCallbacks) {
				pair.second(x, y);
			}

			if (actionMasked == AMOTION_EVENT_ACTION_DOWN || actionMasked == AMOTION_EVENT_ACTION_POINTER_DOWN) {
				for (std::pair<int, std::function<void(int, int)>> pair : mouseButtonCallbacks) {
					pair.second(0, KEY_PRESSED);
				}
			} else if (actionMasked == AMOTION_EVENT_ACTION_UP || actionMasked == AMOTION_EVENT_ACTION_POINTER_UP) {
				for (std::pair<int, std::function<void(int, int)>> pair : mouseButtonCallbacks) {
					pair.second(0, KEY_RELEASED);
				}
			}
			return true;
		} else if (type == AINPUT_EVENT_TYPE_KEY) {
			int32_t action = AKeyEvent_getAction(event);
			int32_t keyCode = AKeyEvent_getKeyCode(event);
			if (action == AKEY_EVENT_ACTION_DOWN) {
				for (std::pair<int, std::function<void(int, int)>> pair : mouseButtonCallbacks) {
					pair.second(keyCode, KEY_PRESSED);
				}
			} else if (action == AKEY_EVENT_ACTION_UP) {
				for (std::pair<int, std::function<void(int, int)>> pair : mouseButtonCallbacks) {
					pair.second(keyCode, KEY_RELEASED);
				}
			}
			return true;
		}
		return false;
	}
#endif
int Input::addMouseButtonCallback(std::function<void(int, int)> cb) {
	int id = nextMouseButtonId++;
	mouseButtonCallbacks[id] = cb;
	return id;
}
void Input::removeMouseButtonCallback(int id) {
	mouseButtonCallbacks.erase(id);
}
int Input::addMousePosCallback(std::function<void(double, double)> cb) {
	int id = nextMousePosId++;
	mousePosCallbacks[id] = cb;
	return id;
}
void Input::removeMousePosCallback(int id) {
	mousePosCallbacks.erase(id);
}
int Input::addKeyboardCallback(std::function<void(int, int)> cb) {
	int id = nextKeyboardId++;
	keyboardCallbacks[id] = cb;
	return id;
}
void Input::removeKeyboardCallback(int id) {
	keyboardCallbacks.erase(id);
}
#ifdef _WIN32
	Input::Input(GLFWwindow* win) {
		window = win;
		input = this;
		glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int b, int a, int m) {input->mouse_button(w, b, a, m); });
		glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {input->mouse_move(w, x, y); });
		glfwSetKeyCallback(window, [](GLFWwindow* w, int k, int s, int a, int m) {input->key_callback(w, k, a, m); });
	}
	Input::~Input() {
		mouseButtonCallbacks.clear();
		mousePosCallbacks.clear();
		keyboardCallbacks.clear();
		glfwSetMouseButtonCallback(window, nullptr);
		glfwSetCursorPosCallback(window, nullptr);
		glfwSetKeyCallback(window, nullptr);
	}
#else
	Input::Input(void* win) {
		input = this;
	}
	Input::~Input() {
		mouseButtonCallbacks.clear();
		mousePosCallbacks.clear();
		keyboardCallbacks.clear();
	}
#endif