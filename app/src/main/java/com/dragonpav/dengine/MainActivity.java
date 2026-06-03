package com.dragonpav.dengine;

import androidx.appcompat.app.AppCompatActivity;

import android.opengl.EGL15;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;

public class MainActivity extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        GLSurfaceView gsv = new GLSurfaceView(this);
        gsv.setEGLContextFactory(new GLSurfaceView.EGLContextFactory() {
            @Override
            public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig) {
                int[] attribs = {
                        EGL15.EGL_CONTEXT_MAJOR_VERSION, 3,
                        EGL15.EGL_CONTEXT_MINOR_VERSION, 2,
                        EGL10.EGL_NONE
                };
                return egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, attribs);
            }
            @Override
            public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context) {
                egl.eglDestroyContext(display, context);
            }
        });
        Renderer rend = new Renderer(getAssets());
        gsv.setRenderer(rend);
        gsv.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                rend.onTouch(event.getAction(), (int) event.getX(), (int) event.getY());
                return true;
            }
        });
        setContentView(gsv);
    }
}