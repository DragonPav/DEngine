#include "Camera.h"
void Camera::ortho(float left, float right, float bottom, float top, float near, float far) {
	projection.values = glm::ortho(left, right, bottom, top, near, far);
}
void Camera::perspective(float fov, float aspectRatio, float near, float far) {
	projection.values = glm::perspective(glm::radians(fov), aspectRatio, near, far);
}
void Camera::lookAt(Vector3 pos) {
	lookAtPos = pos;
	view.values = glm::lookAt(glm::vec3(camPos.getX(), camPos.getY(), camPos.getZ()), glm::vec3(lookAtPos.getX(), lookAtPos.getY(), lookAtPos.getZ()), glm::vec3(0, 1.0f, 0));
}
void Camera::setCameraPos(Vector3 pos) {
	camPos = pos;
	view.values = glm::lookAt(glm::vec3(camPos.getX(), camPos.getY(), camPos.getZ()), glm::vec3(lookAtPos.getX(), lookAtPos.getY(), lookAtPos.getZ()), glm::vec3(0, 1.0f, 0));
}
void Camera::setProjection(Matrix4 proj) {
	projection = proj;
}
void Camera::setView(Matrix4 vi) {
	view = vi;
}
void Camera::setAutoUpdate(bool flag) {
	autoUpdate = flag;
}
void Camera::setViewportWidth(int w) {
	viewportWidth = w;
}
void Camera::setViewportHeight(int h) {
	viewportHeight = h;
}
void Camera::setAudioManager(AudioManager* man) {
	audioMan = man;
}
Vector3& Camera::getCameraPos() {
	return camPos;
}
Vector3& Camera::getLookPos() {
	return lookAtPos;
}
Matrix4& Camera::getProjection() {
	return projection;
}
Matrix4& Camera::getView() {
	return view;
}
bool Camera::isAutoUpdate() {
	return autoUpdate;
}
RenderUtils* Camera::getRenderUtils() {
	return rendUtils;
}
int Camera::getViewportWidth() {
	return viewportWidth;
}
int Camera::getViewportHeight() {
	return viewportHeight;
}
void Camera::update() {
	projection.uniform();
	view.uniform();
}
void Camera::setCameraControl(CameraControl* cc) {
	cc->setup(this);
}
#ifdef _WIN32
	Vector3 Camera::getMouseWorldRay(float mouseX, float mouseY) {
		int width, height;
		glfwGetFramebufferSize(rendUtils->getWindow(), &width, &height);
		glm::vec4 viewport(0, 0, (float)width, (float)height);
		glm::vec3 nearPoint = glm::unProject(glm::vec3(mouseX, mouseY, 0.0f), view.values, projection.values, viewport);
		glm::vec3 farPoint = glm::unProject(glm::vec3(mouseX, mouseY, 1.0f), view.values, projection.values, viewport);
		glm::vec3 vec = glm::normalize(farPoint - nearPoint);
		return Vector3(vec.x, vec.y, vec.z);
	}
	bool Camera::isMouseOverBoundingBox(float mouseX, float mouseY, BoundingBox* bounds) {
		Vector3 rayDir = getMouseWorldRay(mouseX, mouseY);
		glm::vec3 origin(camPos.getX(), camPos.getY(), camPos.getZ());
		glm::vec3 invDir = 1.0f / glm::vec3(rayDir.getX(), rayDir.getY(), rayDir.getZ());
		glm::vec3 t1 = (glm::vec3(bounds->getMin().getX(), bounds->getMin().getY(), bounds->getMin().getZ()) - origin) * invDir;
		glm::vec3 t2 = (glm::vec3(bounds->getMax().getX(), bounds->getMax().getY(), bounds->getMax().getZ()) - origin) * invDir;
		glm::vec3 tMin = glm::min(t1, t2);
		glm::vec3 tMax = glm::max(t1, t2);
		float tNear = glm::max(glm::max(tMin.x, tMin.y), tMin.z);
		float tFar = glm::min(glm::min(tMax.x, tMax.y), tMax.z);
		return tNear <= tFar && tFar >= 0;
	}
#else
	Vector3 Camera::getMouseWorldRay(float mouseX, float mouseY, float width, float height) {
		glm::vec4 viewport(0, 0, (float)width, (float)height);
		glm::vec3 nearPoint = glm::unProject(glm::vec3(mouseX, mouseY, 0.0f), view.values, projection.values, viewport);
		glm::vec3 farPoint = glm::unProject(glm::vec3(mouseX, mouseY, 1.0f), view.values, projection.values, viewport);
		glm::vec3 vec = glm::normalize(farPoint - nearPoint);
		return Vector3(vec.x, vec.y, vec.z);
	}
	bool Camera::isMouseOverBoundingBox(float mouseX, float mouseY, BoundingBox* bounds, float scrW, float scrH) {
		Vector3 rayDir = getMouseWorldRay(mouseX, mouseY, scrW, scrH);
		glm::vec3 origin(camPos.getX(), camPos.getY(), camPos.getZ());
		glm::vec3 invDir = 1.0f / glm::vec3(rayDir.getX(), rayDir.getY(), rayDir.getZ());
		glm::vec3 t1 = (glm::vec3(bounds->getMin().getX(), bounds->getMin().getY(), bounds->getMin().getZ()) - origin) * invDir;
		glm::vec3 t2 = (glm::vec3(bounds->getMax().getX(), bounds->getMax().getY(), bounds->getMax().getZ()) - origin) * invDir;
		glm::vec3 tMin = glm::min(t1, t2);
		glm::vec3 tMax = glm::max(t1, t2);
		float tNear = glm::max(glm::max(tMin.x, tMin.y), tMin.z);
		float tFar = glm::min(glm::min(tMax.x, tMax.y), tMax.z);
		return tNear <= tFar && tFar >= 0;
	}
#endif
void Camera::updateFrustum() {
	if (rendUtils->getLighting() != nullptr) {
		rendUtils->getLighting()->cameraPos.setX(camPos.getX());
		rendUtils->getLighting()->cameraPos.setY(camPos.getY());
		rendUtils->getLighting()->cameraPos.setZ(camPos.getZ());
		rendUtils->getLighting()->cameraPos.uniform();
	}
	glm::mat4 vp = projection.values * view.values;
	frustumPlanes[0].normal = glm::vec3(vp[0][3] + vp[0][0], vp[1][3] + vp[1][0], vp[2][3] + vp[2][0]);
	frustumPlanes[0].distance = vp[3][3] + vp[3][0];
	frustumPlanes[1].normal = glm::vec3(vp[0][3] - vp[0][0], vp[1][3] - vp[1][0], vp[2][3] - vp[2][0]);
	frustumPlanes[1].distance = vp[3][3] - vp[3][0];
	frustumPlanes[2].normal = glm::vec3(vp[0][3] + vp[0][1], vp[1][3] + vp[1][1], vp[2][3] + vp[2][1]);
	frustumPlanes[2].distance = vp[3][3] + vp[3][1];
	frustumPlanes[3].normal = glm::vec3(vp[0][3] - vp[0][1], vp[1][3] - vp[1][1], vp[2][3] - vp[2][1]);
	frustumPlanes[3].distance = vp[3][3] - vp[3][1];
	frustumPlanes[4].normal = glm::vec3(vp[0][3] + vp[0][2], vp[1][3] + vp[1][2], vp[2][3] + vp[2][2]);
	frustumPlanes[4].distance = vp[3][3] + vp[3][2];
	frustumPlanes[5].normal = glm::vec3(vp[0][3] - vp[0][2], vp[1][3] - vp[1][2], vp[2][3] - vp[2][2]);
	frustumPlanes[5].distance = vp[3][3] - vp[3][2];
	for (int i = 0; i < 6; ++i) {
		float length = glm::length(frustumPlanes[i].normal);
		if (length > 0.0f) {
			frustumPlanes[i].normal /= length;
			frustumPlanes[i].distance /= length;
		}
	}
	if (audioMan != nullptr) {
		float fwdX = lookAtPos.getX() - camPos.getX();
		float fwdY = lookAtPos.getY() - camPos.getY();
		float fwdZ = lookAtPos.getZ() - camPos.getZ();
		float length = sqrtf(fwdX * fwdX + fwdY * fwdY + fwdZ * fwdZ);
		if (length > 0.0001f) {
			fwdX /= length;
			fwdY /= length;
			fwdZ /= length;
		} else {
			fwdX = 0.0f; fwdY = 0.0f; fwdZ = -1.0f;
		}
		audioMan->update(camPos, Vector3(fwdX, fwdY, fwdZ), Vector3(0, 1.0f, 0));
	}
}
bool Camera::isBoxVisible(BoundingBox bounds) {
	glm::vec3 min(bounds.getMin().getX(), bounds.getMin().getY(), bounds.getMin().getZ());
	glm::vec3 max(bounds.getMax().getX(), bounds.getMax().getY(), bounds.getMax().getZ());

	for (int i = 0; i < 6; ++i) {
		glm::vec3 p = min;
		if (frustumPlanes[i].normal.x >= 0) p.x = max.x;
		if (frustumPlanes[i].normal.y >= 0) p.y = max.y;
		if (frustumPlanes[i].normal.z >= 0) p.z = max.z;

		float signedDistance = glm::dot(frustumPlanes[i].normal, p) + frustumPlanes[i].distance;
		if (signedDistance < 0) {
			return false;
		}
	}
	return true;
}
AudioManager* Camera::getAudioManager() {
	return audioMan;
}
Camera::Camera(RenderUtils* re) {
	rendUtils = re;
	projection = Matrix4(glGetUniformLocation(re->getProgram()->getId(), "projection"));
	view = Matrix4(glGetUniformLocation(re->getProgram()->getId(), "view"));
	camPos = Vector3(0, 0, 2);
	lookAtPos = Vector3(0, 0, 0);
}