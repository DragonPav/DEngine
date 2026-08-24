#pragma once
#include "Matrix4.h"
#include "RenderUtils.h"
#include "CameraControl.h"
#include "BoundingBox.h"
#include "AudioManager.h"
#ifdef _WIN32
	#include <GL/glew.h>
#else
	#include <GLES3/gl32.h>
#endif
#undef near
#undef far
class RenderUtils;
class CameraControl;
class AudioManager;
struct CameraPlane {
	glm::vec3 normal;
	float distance;
};
class Camera {
private:
	Matrix4 projection;
	Matrix4 view;
	Vector3 camPos;
	Vector3 lookAtPos;
	int viewportWidth = 0;
	int viewportHeight = 0;
	bool autoUpdate = true;
	RenderUtils* rendUtils = nullptr;
	CameraPlane frustumPlanes[6];
	AudioManager* audioMan = nullptr;
public:
	void ortho(float left, float right, float bottom, float top, float near, float far);
	void perspective(float fov, float aspectRatio, float near, float far);
	void lookAt(Vector3 pos);
	void setCameraPos(Vector3 pos);
	void setProjection(Matrix4 proj);
	void setView(Matrix4 vi);
	void setAutoUpdate(bool flag);
	void setViewportWidth(int w);
	void setViewportHeight(int h);
	void setAudioManager(AudioManager* man);
	Vector3& getLookPos();
	Vector3& getCameraPos();
	Matrix4& getProjection();
	Matrix4& getView();
	bool isAutoUpdate();
	RenderUtils* getRenderUtils();
	int getViewportWidth();
	int getViewportHeight();
	void update();
	void setCameraControl(CameraControl* cc);
	#ifdef _WIN32
		Vector3 getMouseWorldRay(float mouseX, float mouseY);
		bool isMouseOverBoundingBox(float mouseX, float mouseY, BoundingBox* bounds);
	#else
		Vector3 getMouseWorldRay(float mouseX, float mouseY, float width, float height);
		bool isMouseOverBoundingBox(float mouseX, float mouseY, BoundingBox* bounds, float scrW, float scrH);
	#endif
	void updateFrustum();
	bool isBoxVisible(BoundingBox bb);
	AudioManager* getAudioManager();
	Camera() = delete;
	Camera(RenderUtils* re);
};