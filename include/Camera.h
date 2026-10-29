#pragma once
#include "Matrix4.h"
#include "RenderUtils.h"
#include "CameraControl.h"
#include "BoundingBox.h"
#include "AudioManager.h"
#ifndef __ANDROID__
	#include <GL/glew.h>
#else
	#include <GLES3/gl32.h>
#endif
#undef near
#undef far
class RenderUtils;
class CameraControl;
class AudioManager;
/** @brief Represents plane of `Camera` **/
struct CameraPlane {
	glm::vec3 normal;
	float distance;
};
/** @class Camera
 * @brief Used for engine translations
 **/
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
	/** @brief Create orthographic projection matrix */
	void ortho(float left, float right, float bottom, float top, float near, float far);
	/** @brief Create perspective projection matrix */
	void perspective(float fov, float aspectRatio, float near, float far);
	/** @brief Set `Camera` position where it looks at */
	void lookAt(Vector3 pos);
	/** @brief Set `Camera` position */
	void setCameraPos(Vector3 pos);
	/** @brief Set `Camera` projection matrix */
	void setProjection(Matrix4 proj);
	/** @brief Set `Camera` view matrix */
	void setView(Matrix4 vi);
	/** @brief Set `Camera` auto update flag, it means every `Object3D` can update `Camera` at render */
	void setAutoUpdate(bool flag);
	/** @brief Set `Camera` viewport width */
	void setViewportWidth(int w);
	/** @brief Set `Camera` viewport height */
	void setViewportHeight(int h);
	/** @brief Attach `AudioManager` to `Camera` */
	void setAudioManager(AudioManager* man);
	/** @brief Get `Camera` position where it looks at */
	Vector3& getLookPos();
	/** @brief Get `Camera` position */
	Vector3& getCameraPos();
	/** @brief Get `Camera` projection matrix */
	Matrix4& getProjection();
	/** @brief Get `Camera` view matrix */
	Matrix4& getView();
	/** @brief Get `Camera` auto update flag */
	bool isAutoUpdate();
	/** @brief Get `Camera` `RenderUtils` */
	RenderUtils* getRenderUtils();
	/** @brief Get `Camera` viewport width */
	int getViewportWidth();
	/** @brief Get `Camera` viewport height */
	int getViewportHeight();
	/** @brief Force update `Camera`
	 * 
	 * Must be called for update, if auto update flag set to `false`
	*/
	void update();
	/** @brief Bind `CameraControl` to `Camera` */
	void setCameraControl(CameraControl* cc);
	#ifndef __ANDROID__
		/** @brief Get mouse world ray */
		Vector3 getMouseWorldRay(float mouseX, float mouseY);
		/** @brief Check if mouse over `BoundingBox` */
		bool isMouseOverBoundingBox(float mouseX, float mouseY, BoundingBox* bounds);
	#else
		/** @brief Get mouse world ray */
		Vector3 getMouseWorldRay(float mouseX, float mouseY, float width, float height);
		/** @brief Check if mouse over `BoundingBox`
		 * @param scrW Screen width
		 * @param srcH Screen height
		*/
		bool isMouseOverBoundingBox(float mouseX, float mouseY, BoundingBox* bounds, float scrW, float scrH);
	#endif
	/** @brief Not `update()`, updates `Camera` position for bonded `AudioManager` and frustum culling */
	void updateFrustum();
	/** @brief Check if `BoundingBox` visible in the `Camera` frustum */
	bool isBoxVisible(BoundingBox bb);
	/** @brief Return bonded audio manager 
	 * @return `AudioManager` or `nullptr`
	**/
	AudioManager* getAudioManager();
	Camera() = delete;
	Camera(RenderUtils* re);
};