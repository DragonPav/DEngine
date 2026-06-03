#pragma once
#include "Matrix4.h"
#include "RenderUtils.h"
#include "CameraControl.h"
#include "BoundingBox.h"
#include <GLES3/gl32.h>
class RenderUtils;
class CameraControl;
class Camera {
public:
	Matrix4 projection;
	Matrix4 view;
	Vector3 camPos;
	Vector3 lookAtPos;
	bool autoUpdate = true;
	RenderUtils* rendUtils = nullptr;
	int screenWidth, screenHeight;
	float mousePosX = 0, mousePosY = 0;
	void ortho(float left, float right, float bottom, float top, float near, float far);
	void perspective(float fov, float aspectRatio, float near, float far);
	void lookAt(Vector3 pos);
	void setPos(Vector3 pos);
	void update();
	void setCameraControl(CameraControl* cc);
	Vector3 getMouseWorldRay(float mouseX, float mouseY);
	bool isMouseOverBoundingBox(float mouseX, float mouseY, BoundingBox* bounds);
	Camera() = delete;
	Camera(RenderUtils* re);
};