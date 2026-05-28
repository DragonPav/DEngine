package com.dragonpav.dengine;

import android.annotation.SuppressLint;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;
import android.view.View;

public class CameraTouchControl {
	private float lastX, lastY;
	private GLSurfaceView gsv;
	private float yaw = -90.0f;
	private float pitch = 0.0f;
	private float radius = 4.0f;
	private float sensitivity = 0.2f;
	private float minRadius = 1.0f;
	private float maxRadius = 50.0f;
	private float pinchSensitivity = 0.01f;
	private float lastPinchDist = -1f;
	public CameraTouchControl(RenderUtils renderUtils) {
		gsv = renderUtils.gsv;
	}
	private float getPinchDist(MotionEvent event) {
		float dx = event.getX(0) - event.getX(1);
		float dy = event.getY(0) - event.getY(1);
		return (float) Math.sqrt(dx * dx + dy * dy);
	}
	@SuppressLint("ClickableViewAccessibility")
	protected void setup(Camera camera) {
		float dx = camera.camPos.x - camera.lookAtPos.x;
		float dy = camera.camPos.y - camera.lookAtPos.y;
		float dz = camera.camPos.z - camera.lookAtPos.z;
		radius = (float) Math.sqrt(dx * dx + dy * dy + dz * dz);
		gsv.setOnTouchListener(new View.OnTouchListener() {
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				int action = event.getActionMasked();
				if (event.getPointerCount() == 2) {
					if (action == MotionEvent.ACTION_POINTER_DOWN) {
						lastPinchDist = getPinchDist(event);
					} else if (action == MotionEvent.ACTION_MOVE) {
						float dist = getPinchDist(event);
						if (lastPinchDist > 0) {
							float delta = (lastPinchDist - dist) * pinchSensitivity;
							radius += delta * radius;
							if (radius < minRadius) radius = minRadius;
							if (radius > maxRadius) radius = maxRadius;
							updateCameraPos(camera);
						}
						lastPinchDist = dist;
					} else if (action == MotionEvent.ACTION_POINTER_UP) {
						lastPinchDist = -1f;
					}
				} else if (event.getPointerCount() == 1) {
					if (action == MotionEvent.ACTION_DOWN) {
						camera.pointerPosX = event.getX();
						camera.pointerPosY = event.getY();
						lastX = event.getX();
						lastY = event.getY();
					} else if (action == MotionEvent.ACTION_MOVE) {
						float x = event.getX();
						float y = event.getY();
						float xoffset = (x - lastX) * sensitivity;
						float yoffset = (y - lastY) * sensitivity;
						lastX = x;
						lastY = y;
						yaw += xoffset;
						pitch += yoffset;
						if (pitch > 89.0f) pitch = 89.0f;
						if (pitch < -89.0f) pitch = -89.0f;
						updateCameraPos(camera);
					} else if (action == MotionEvent.ACTION_UP) {
						camera.pointerPosX = -1;
						camera.pointerPosY = -1;
					}
				}
				return true;
			}
		});
	}
	private void updateCameraPos(Camera camera) {
		float yawRad = (float) Math.toRadians(yaw);
		float pitchRad = (float) Math.toRadians(pitch);
		Values.Vector3 newPos = new Values.Vector3();
		newPos.x = camera.lookAtPos.x + radius * (float) Math.cos(pitchRad) * (float) Math.cos(yawRad);
		newPos.y = camera.lookAtPos.y + radius * (float) Math.sin(pitchRad);
		newPos.z = camera.lookAtPos.z + radius * (float) Math.cos(pitchRad) * (float) Math.sin(yawRad);
		camera.setPos(newPos);
		if (camera.rendUtils.lighting != null) {
			camera.rendUtils.lighting.cameraPos.x = newPos.x;
			camera.rendUtils.lighting.cameraPos.y = newPos.y;
			camera.rendUtils.lighting.cameraPos.z = newPos.z;
			camera.rendUtils.lighting.cameraPos.uniform();
		}
	}
	public void setSensitivity(float sense) {
		sensitivity = sense;
	}
	public void setPinchSensitivity(float sense) {
		pinchSensitivity = sense;
	}
	public void setRadiusLimits(float min, float max) {
		minRadius = min;
		maxRadius = max;
	}
}
