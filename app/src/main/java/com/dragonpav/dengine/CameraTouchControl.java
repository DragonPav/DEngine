package com.dragonpav.dengine;

import android.opengl.GLSurfaceView;
import android.view.MotionEvent;
import android.view.View;

public class CameraTouchControl {
	private float startX, startY;
	private GLSurfaceView gsv;
	public CameraTouchControl(RenderUtils renderUtils) {
		gsv = renderUtils.gsv;
	}
	public void setup(Camera camera) {
		gsv.setOnTouchListener(new View.OnTouchListener() {
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				if (event.getAction() == MotionEvent.ACTION_DOWN) {
					startX = event.getX();
					startY = event.getY();
				} else if (event.getAction() == MotionEvent.ACTION_MOVE) {
					float x = event.getX();
					float y = event.getY();
					if (x > startX) {
						if (x - startX >= 80) {
							camera.view.rotate(new Values.Vector3(0, 1, 0), (x - startX) / 100, null);
						}
					} else if (x < startX) {
						if (startX - x >= 80) {
							camera.view.rotate(new Values.Vector3(0, 1, 0), -(startX - x) / 100, null);
						}
					}
					if (y > startY) {
						if (y - startY >= 80) {
							camera.view.rotate(new Values.Vector3(1, 0, 0), (y - startY) / 100, null);
						}
					} else if (y < startY) {
						if (startY - y >= 80) {
							camera.view.rotate(new Values.Vector3(1, 0, 0), -(startY - y) / 100, null);
						}
					}
				}
				return true;
			}
		});
	}
}