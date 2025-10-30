package com.dragonpav.dengine;

import android.opengl.GLES30;
import android.opengl.Matrix;

public class Camera {
	public Values.Matrix4 projection;
	public Values.Matrix4 view;
	public Values.Vector3 camPos;
	public Values.Vector3 lookAtPos;
	public boolean autoUpdate = true;
	public RenderUtils rendUtils;
	public void ortho(float left, float right, float bottom, float top, float near, float far) {
		Matrix.orthoM(projection.values, 0, left, right, bottom, top, near, far);
	}
	public void perspective(float fov, float aspectRatio, float near, float far) {
		Matrix.perspectiveM(projection.values, 0, fov, aspectRatio, near, far);
	}
	public void lookAt(Values.Vector3 pos) {
		lookAtPos = pos;
		Matrix.setLookAtM(view.values, 0, camPos.x, camPos.y, camPos.z, lookAtPos.x, lookAtPos.y, lookAtPos.z, 0, 1, 0);
	}
	public void setPos(Values.Vector3 pos) {
		camPos = pos;
		Matrix.setLookAtM(view.values, 0, camPos.x, camPos.y, camPos.z, lookAtPos.x, lookAtPos.y, lookAtPos.z, 0, 1, 0);
	}
	public void setCameraTouchControl(CameraTouchControl ctt) {
		ctt.setup(this);
	}
	public void update() {
		projection.uniform();
		view.uniform();
	}
	public Camera(RenderUtils re) {
		rendUtils = re;
		projection = new Values.Matrix4(GLES30.glGetUniformLocation(re.p.programId, "projection"));
		view = new Values.Matrix4(GLES30.glGetUniformLocation(re.p.programId, "view"));
		camPos = new Values.Vector3(0, 0, 2);
		lookAtPos = new Values.Vector3();
	}
}