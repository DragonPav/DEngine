#pragma once
#include "Camera.h"
#include "RenderUtils.h"
#include <GLES3/gl32.h>
class Camera;
class RenderUtils;
class CameraControl {
public:
	void setup(Camera* camera);
	void setSensitivity(float sense);
	void onTouch(int action, float x, float y);
	CameraControl() = default;
};