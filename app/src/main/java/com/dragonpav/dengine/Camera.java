package com.dragonpav.dengine;

import android.opengl.GLES30;

import glm_.glm;
import glm_.vec3.Vec3;
import glm_.vec4.Vec4i;
public class Camera {
	public Values.Matrix4 projection;
	public Values.Matrix4 view;
	public Values.Vector3 camPos;
	public Values.Vector3 lookAtPos;
	public boolean autoUpdate = true;
	public RenderUtils rendUtils;
	public int screenWidth, screenHeight;
	public float pointerPosX, pointerPosY;
	public void ortho(float left, float right, float bottom, float top, float near, float far) {
		projection.values = glm.INSTANCE.ortho(left, right, bottom, top, near, far);
	}
	public void perspective(float fov, float aspectRatio, float near, float far) {
		projection.values = glm.INSTANCE.perspective(glm.INSTANCE.radians(fov), aspectRatio, near, far);
	}
	public void lookAt(Values.Vector3 pos) {
		lookAtPos = pos;
		view.values = glm.INSTANCE.lookAt(new Vec3(camPos.x, camPos.y, camPos.z), new Vec3(lookAtPos.x, lookAtPos.y, lookAtPos.z), new Vec3(0, 1, 0));
	}
	public void setPos(Values.Vector3 pos) {
		camPos = pos;
		view.values = glm.INSTANCE.lookAt(new Vec3(camPos.x, camPos.y, camPos.z), new Vec3(lookAtPos.x, lookAtPos.y, lookAtPos.z), new Vec3(0, 1, 0));
	}
	public void setCameraTouchControl(CameraTouchControl ctt) {
		ctt.setup(this);
	}
	public void update() {
		projection.uniform();
		view.uniform();
	}
	public Values.Vector3 getPointerWorldRay(float pointerX, float pointerY) {
		Vec4i viewport = new Vec4i(0, 0, (float) screenWidth, (float) screenHeight);
		float flippedY = screenHeight - pointerY;
		Vec3 nearPoint = glm.INSTANCE.unProject(new Vec3(pointerX, flippedY, 0), view.values, projection.values, viewport);
		Vec3 farPoint = glm.INSTANCE.unProject(new Vec3(pointerX, flippedY, 1.0f), view.values, projection.values, viewport);
		Vec3 dir = new Vec3(
				farPoint.getX() - nearPoint.getX(),
				farPoint.getY() - nearPoint.getY(),
				farPoint.getZ() - nearPoint.getZ()
		);
		Vec3 vec = new Vec3();
		glm.INSTANCE.normalize(dir, vec);
		return new Values.Vector3(vec.getX(), vec.getY(), vec.getZ());
	}
	public boolean isPointerOverBoundingBox(float pointerX, float pointerY, BoundingBox bounds) {
		if (pointerX < 0 || pointerY < 0) return false;
		Values.Vector3 rayDir = getPointerWorldRay(pointerX, pointerY);
		Vec3 origin = new Vec3(camPos.x, camPos.y, camPos.z);
		Vec3 invDir = new Vec3(1.0f / rayDir.x,
				1.0f / rayDir.y,
				1.0f / rayDir.z);
		Vec3 bMin = new Vec3(bounds.min.x, bounds.min.y, bounds.min.z);
		Vec3 bMax = new Vec3(bounds.max.x, bounds.max.y, bounds.max.z);
		Vec3 t1 = bMin.minus(origin).times(invDir);
		Vec3 t2 = bMax.minus(origin).times(invDir);
		Vec3 tMin = glm.INSTANCE.min(t1, t2);
		Vec3 tMax = glm.INSTANCE.max(t1, t2);
		float tNear = glm.INSTANCE.max(glm.INSTANCE.max(tMin.getX(), tMin.getY()), tMin.getZ());
		float tFar = glm.INSTANCE.min(glm.INSTANCE.min(tMax.getX(), tMax.getY()), tMax.getZ());
		return tNear <= tFar && tFar >= 0;
	}
	public Camera(RenderUtils re) {
		screenWidth = re.gsv.getWidth();
		screenHeight = re.gsv.getHeight();
		rendUtils = re;
		projection = new Values.Matrix4(GLES30.glGetUniformLocation(re.p.programId, "projection"));
		view = new Values.Matrix4(GLES30.glGetUniformLocation(re.p.programId, "view"));
		camPos = new Values.Vector3(0, 0, 2);
		lookAtPos = new Values.Vector3();
	}
}