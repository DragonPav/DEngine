#include "Camera.h"
void Camera::ortho(float left, float right, float bottom, float top, float near, float far) {
	projection.values = glm::ortho(left, right, bottom, top, near, far);
}
void Camera::perspective(float fov, float aspectRatio, float near, float far) {
	projection.values = glm::perspective(glm::radians(fov), aspectRatio, near, far);
}
void Camera::lookAt(Vector3 pos) {
	lookAtPos = pos;
	view.values = glm::lookAt(glm::vec3(camPos.x, camPos.y, camPos.z), glm::vec3(lookAtPos.x, lookAtPos.y, lookAtPos.z), glm::vec3(0, 1.0f, 0));
}
void Camera::setPos(Vector3 pos) {
	camPos = pos;
	view.values = glm::lookAt(glm::vec3(camPos.x, camPos.y, camPos.z), glm::vec3(lookAtPos.x, lookAtPos.y, lookAtPos.z), glm::vec3(0, 1.0f, 0));
}
void Camera::update() {
	projection.uniform();
	view.uniform();
}
void Camera::setCameraControl(CameraControl* cc) {
	cc->setup(this);
}
Vector3 Camera::getMouseWorldRay(float mouseX, float mouseY) {
	glm::vec4 viewport(0, 0, (float)screenWidth, (float)screenHeight);
	glm::vec3 nearPoint = glm::unProject(glm::vec3(mouseX, mouseY, 0.0f), view.values, projection.values, viewport);
	glm::vec3 farPoint = glm::unProject(glm::vec3(mouseX, mouseY, 1.0f), view.values, projection.values, viewport);
	glm::vec3 vec = glm::normalize(farPoint - nearPoint);
	return Vector3(vec.x, vec.y, vec.z);
}
bool Camera::isMouseOverBoundingBox(float mouseX, float mouseY, BoundingBox* bounds) {
	Vector3 rayDir = getMouseWorldRay(mouseX, mouseY);
	glm::vec3 origin(camPos.x, camPos.y, camPos.z);
	glm::vec3 invDir = 1.0f / glm::vec3(rayDir.x, rayDir.y, rayDir.z);
	glm::vec3 t1 = (glm::vec3(bounds->min.x, bounds->min.y, bounds->min.z) - origin) * invDir;
	glm::vec3 t2 = (glm::vec3(bounds->max.x, bounds->max.y, bounds->max.z) - origin) * invDir;
	glm::vec3 tMin = glm::min(t1, t2);
	glm::vec3 tMax = glm::max(t1, t2);
	float tNear = glm::max(glm::max(tMin.x, tMin.y), tMin.z);
	float tFar = glm::min(glm::min(tMax.x, tMax.y), tMax.z);
	return tNear <= tFar && tFar >= 0;
}
Camera::Camera(RenderUtils* re) {
	screenWidth = re->width;
	screenHeight = re->height;
	rendUtils = re;
	projection = Matrix4(glGetUniformLocation(re->p->programId, "projection"));
	view = Matrix4(glGetUniformLocation(re->p->programId, "view"));
	camPos = Vector3(0, 0, 2);
	lookAtPos = Vector3(0, 0, 0);
}