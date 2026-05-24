package com.dragonpav.dengine;

import android.opengl.GLES20;
import android.opengl.GLES30;
import android.opengl.GLSurfaceView;
import android.util.Log;

import androidx.annotation.Nullable;

import java.nio.FloatBuffer;
import java.nio.IntBuffer;

public class RenderUtils {
	public Program p;
	public Lighting lighting;
	public GLSurfaceView gsv;
	public ObjectBuffer createStaticVAO(FloatBuffer vertices, IntBuffer indices, FloatBuffer tex, FloatBuffer normalBuffer, int vertBufSize, int indexBufSize, int texBufSize, int normalBufferSize) {
		ObjectBuffer b = new ObjectBuffer();
		int[] tmpBuf = new int[1];
		GLES30.glGenVertexArrays(1, tmpBuf, 0);
		b.VAO = tmpBuf[0];
		GLES30.glBindVertexArray(b.VAO);
		GLES30.glGenBuffers(1, tmpBuf, 0);
		b.VBO = tmpBuf[0];
		GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, b.VBO);
		GLES30.glBufferData(GLES30.GL_ARRAY_BUFFER, vertBufSize, vertices, GLES30.GL_STATIC_DRAW);
		GLES30.glVertexAttribPointer(0, 3, GLES30.GL_FLOAT, false, 3 * 4, 0);
		GLES30.glEnableVertexAttribArray(0);
		GLES30.glGenBuffers(1, tmpBuf, 0);
		b.VBOtexture = tmpBuf[0];
		GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, b.VBOtexture);
		GLES30.glBufferData(GLES30.GL_ARRAY_BUFFER, texBufSize, tex, GLES30.GL_STATIC_DRAW);
		GLES30.glVertexAttribPointer(1, 2, GLES30.GL_FLOAT, false, 2 * 4, 0);
		GLES30.glEnableVertexAttribArray(1);
		if (normalBuffer != null && normalBufferSize > 0) {
			GLES30.glGenBuffers(1, tmpBuf, 0);
			b.VBOnormal = tmpBuf[0];
			GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, b.VBOnormal);
			GLES30.glBufferData(GLES30.GL_ARRAY_BUFFER, normalBufferSize, normalBuffer, GLES30.GL_STATIC_DRAW);
			GLES30.glVertexAttribPointer(2, 3, GLES30.GL_FLOAT, false, 3 * 4, 0);
			GLES30.glEnableVertexAttribArray(2);
		} else {
			b.VBOnormal = 0;
		}
		GLES30.glGenBuffers(1, tmpBuf, 0);
		b.EBO = tmpBuf[0];
		GLES30.glBindBuffer(GLES30.GL_ELEMENT_ARRAY_BUFFER, b.EBO);
		GLES30.glBufferData(GLES30.GL_ELEMENT_ARRAY_BUFFER, indexBufSize, indices, GLES30.GL_STATIC_DRAW);
		GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, 0);
		GLES30.glBindVertexArray(0);
		return b;
	}
	public RenderUtils(Program program, @Nullable Lighting li, GLSurfaceView gsv) {
		p = program;
		lighting = li;
		this.gsv = gsv;
		Log.i("DEngine", "Initializing RenderUtils...");
		Log.i("DEngine", "---------------------------");
		Log.i("DEngine", "GPU: " + GLES30.glGetString(GLES30.GL_RENDERER));
		Log.i("DEngine", "Width: " + gsv.getWidth());
		Log.i("DEngine", "Height: " + gsv.getHeight());
	}
}