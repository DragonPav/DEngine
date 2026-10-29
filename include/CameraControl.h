#pragma once
#include "Camera.h"
#include "RenderUtils.h"
#include "Input.h"
#ifdef __ANDROID__
	#include <android/input.h>
#else
	#include <GLFW/glfw3.h>
#endif
class Camera;
class RenderUtils;
class Input;
/** @class CameraControl
 * @brief Controls `Camera` from user input
*/
class CameraControl {
private:
	void mouseButtonCallback(int button, int action);
	void mousePosCallback(double xpos, double ypos);
public:
	float lastX = 0;
	float lastY = 0;
	Camera* camera = nullptr;
	Input* in = nullptr;
	bool pressed = false;
	float yaw = -90.0f;
	float pitch = 0.0f;
	float radius = 4.0f;
	float sensitivity = 0.01f;
	bool firstPress = false;
	int buttonCbId = -1;
	int moveCbId = -1;
	/** @brief This function is only for Camera */
	void setup(Camera* camera);
	/** @brief Set sentivity of `CameraControl` */
	void setSensitivity(float sense);
	CameraControl() = delete;
	~CameraControl();
	CameraControl(Input* input);
};