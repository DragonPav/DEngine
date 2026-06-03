#include "CameraControl.h"
#include <android/surface_control_input_receiver.h>
double lastX = 0, lastY = 0;
Camera* camera = nullptr;
bool pressed = false;
float yaw = -90.0f;
float pitch = 0.0f;
float radius = 4.0f;
float sensitivity = 0.1f;
void CameraControl::onTouch(int action, float x, float y) {
	if (action == 0) {
		lastX = x;
		lastY = y;
		pressed = true;
	}
	else if (action == 1) {
		pressed = false;
	}
	if (camera != nullptr) {
		camera->mousePosX = x;
		camera->mousePosY = y;
	}
	if (action == 2 && pressed && camera != nullptr) {
		double xoffset = (x - lastX) * sensitivity;
		double yoffset = (y - lastY) * sensitivity;
		lastX = x;
		lastY = y;
		yaw += (float)xoffset;
		pitch += (float)yoffset;
		if (pitch > 89.0f) pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;
		float yawRad = glm::radians(yaw);
		float pitchRad = glm::radians(pitch);
		Vector3 newPos;
		newPos.x = camera->lookAtPos.x + radius * cos(pitchRad) * cos(yawRad);
		newPos.y = camera->lookAtPos.y + radius * sin(pitchRad);
		newPos.z = camera->lookAtPos.z + radius * cos(pitchRad) * sin(yawRad);
		camera->setPos(newPos);
		if (camera->rendUtils->lighting != nullptr) {
			camera->rendUtils->lighting->cameraPos.x = newPos.x;
			camera->rendUtils->lighting->cameraPos.y = newPos.y;
			camera->rendUtils->lighting->cameraPos.z = newPos.z;
		}
	}
}
void CameraControl::setup(Camera* came) {
	camera = came;
	float dx = camera->camPos.x - camera->lookAtPos.x;
	float dy = camera->camPos.y - camera->lookAtPos.y;
	float dz = camera->camPos.z - camera->lookAtPos.z;
	radius = sqrtf(dx * dx + dy * dy + dz * dz);
}
void CameraControl::setSensitivity(float sense) {
	sensitivity = sense;
}