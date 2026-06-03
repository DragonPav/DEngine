package com.dragonpav.dengine;

import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class Renderer implements GLSurfaceView.Renderer {
    static {
        System.loadLibrary("dengine");
    }
    private native void surfaceCreated();
    private native void surfaceChanged(int width, int height);
    private native void drawFrame();
    private native void setAssetMgr(AssetManager assetManager);
    public native void onTouch(int action, int x, int y);
    @Override
    public void onSurfaceCreated(GL10 unused, EGLConfig config) {
        surfaceCreated();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        if (width <= 0 || height <= 0) return;
        surfaceChanged(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        drawFrame();
    }
    public Renderer(AssetManager manager) {
        setAssetMgr(manager);
    }
}
