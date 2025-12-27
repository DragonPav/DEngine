package com.dragonpav.dengine;

import android.content.Context;
import android.opengl.EGL14;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.SurfaceHolder;
import java.util.ArrayList;
import java.util.Iterator;
import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;
import javax.microedition.khronos.opengles.GL10;

import javax.microedition.khronos.egl.EGLContext;
public class Renderer implements GLSurfaceView.Renderer {
	private double frameCount;
	private double deltaTime;
	private long lastFpsTime;
	private long secondTimer;
	private ArrayList<Double> times = new ArrayList<>();
	private double currentFps;
	protected Context context;
	protected GLSurfaceView gsv;
	@Override
	public void onSurfaceCreated(GL10 arg0, EGLConfig arg1) {}
	@Override
	public void onSurfaceChanged(GL10 arg0, int arg1, int arg2) {}
	@Override
	public void onDrawFrame(GL10 arg0) {
		long currentTime = System.currentTimeMillis();
		deltaTime = currentTime - lastFpsTime;
		times.add(deltaTime);
		if (currentTime - secondTimer >= 1000) {
			double sum = 0.0;
			Iterator<Double> iter = times.iterator();
			while (iter.hasNext()) {
				sum += iter.next();
			}
			currentFps = 1000 / (sum / times.size());
			times.clear();
			secondTimer = currentTime;
		}
		lastFpsTime = currentTime;
	}
	protected void onStop() {};
	public double getFPS() {
		return currentFps;
	}
	public Renderer(Context context, GLSurfaceView gsv) {
		this.context = context;
		this.gsv = gsv;
	}
}