#pragma once
#include "Camera.h"
#include "RenderUtils.h"
#include "Input.h"
#ifdef _WIN32
	#include <GLFW/glfw3.h>
#else
	#include <android/input.h>
#endif
class Camera;
class RenderUtils;
class Input;
class CameraControl {
private:
	void mouseButtonCallback(int button, int action);
	void mousePosCallback(double xpos, double ypos);
public:
	float lastX = 0;
	float lastY = 0;
	Camera* camera = nullptr;
	bool pressed = false;
	float yaw = -90.0f;
	float pitch = 0.0f;
	float radius = 4.0f;
	float sensitivity = 0.01f;
	// @brief This function is only for Camera
	void setup(Camera* camera);
	void setSensitivity(float sense);
	CameraControl() = delete;
	CameraControl(Input* input);
};