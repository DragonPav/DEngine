#include "CameraControl.h"
CameraControl* cc = nullptr;
CameraControl::CameraControl(Input* input) {
	input->addMouseButtonCallback([](int button, int action) { cc->mouseButtonCallback(button, action); });
	input->addMousePosCallback([](double xpos, double ypos) { cc->mousePosCallback(xpos, ypos); });
}
void CameraControl::mouseButtonCallback(int button, int action) {
	if (button == 0) {
		if (action == Input::KEY_PRESSED) {
			pressed = true;
			firstPress = true;
		} else if (action == Input::KEY_RELEASED) {
			pressed = false;
		}
	}
}
void CameraControl::mousePosCallback(double xpos, double ypos) {
	if (!pressed || camera == nullptr) return;
	if (firstPress) {
		lastX = xpos;
		lastY = ypos;
		firstPress = false;
	}
	double xoffset = xpos - lastX;
	#ifdef _WIN32
		double yoffset = lastY - ypos;
	#else
		double yoffset = ypos - lastY;
	#endif
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	yaw += (float)xoffset;
	pitch += (float)yoffset;
	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	float yawRad = glm::radians(yaw);
	float pitchRad = glm::radians(pitch);
	Vector3 newPos;
	newPos.setX(camera->getLookPos().getX() + radius * cos(pitchRad) * cos(yawRad));
	newPos.setY(camera->getLookPos().getY() + radius * sin(pitchRad));
	newPos.setZ(camera->getLookPos().getZ() + radius * cos(pitchRad) * sin(yawRad));
	camera->setCameraPos(newPos);
}
void CameraControl::setup(Camera* came) {
	camera = came;
	cc = this;
	float dx = camera->getCameraPos().getX() - camera->getLookPos().getX();
	float dy = camera->getCameraPos().getY() - camera->getLookPos().getY();
	float dz = camera->getCameraPos().getZ() - camera->getLookPos().getZ();
	radius = sqrt(dx * dx + dy * dy + dz * dz);
}
void CameraControl::setSensitivity(float sense) {
	sensitivity = sense;
}