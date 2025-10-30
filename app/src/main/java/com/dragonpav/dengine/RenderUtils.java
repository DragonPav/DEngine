package com.dragonpav.dengine;

import android.opengl.GLES30;
import android.opengl.GLSurfaceView;
import android.util.Log;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;

public class RenderUtils {
	public int VAO, VBO, EBO, VBO_texture, VBO_normal;
	private int[] tmpBuf = new int[1];
	public Program p;
	protected Lighting lighting;
	public GLSurfaceView gsv;
	public void init() {
		Log.i("DEngine", "Initializing RenderUtils...");
		Log.i("DEngine", "---------------------------");
		Log.i("DEngine", "GPU: " + GLES30.glGetString(GLES30.GL_RENDERER));
		Log.i("DEngine", "Width: " + gsv.getWidth());
		Log.i("DEngine", "Height: " + gsv.getHeight());
		tmpBuf = new int[1];
		GLES30.glGenVertexArrays(1, tmpBuf, 0);
		VAO = tmpBuf[0];
		GLES30.glGenBuffers(1, tmpBuf, 0);
		VBO = tmpBuf[0];
		GLES30.glGenBuffers(1, tmpBuf, 0);
		VBO_texture = tmpBuf[0];
		if (lighting != null) {
			GLES30.glGenBuffers(1, tmpBuf, 0);
			VBO_normal = tmpBuf[0];
		}
		GLES30.glGenBuffers(1, tmpBuf, 0);
		EBO = tmpBuf[0];
	}
	public void bindData(FloatBuffer vertices, IntBuffer indices, FloatBuffer tex, int vertBufSize, int indexBufSize, int texBufSize, FloatBuffer normalBuffer, int normalBufferSize) {
		GLES30.glBindVertexArray(VAO);
		GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, VBO);
		GLES30.glBufferData(GLES30.GL_ARRAY_BUFFER, vertBufSize, vertices, GLES30.GL_STATIC_DRAW);
		GLES30.glVertexAttribPointer(0, 3, GLES30.GL_FLOAT, false, 3 * 4, 0);
		GLES30.glEnableVertexAttribArray(0);
		GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, VBO_texture);
		GLES30.glBufferData(GLES30.GL_ARRAY_BUFFER, texBufSize, tex, GLES30.GL_STATIC_DRAW);
		GLES30.glVertexAttribPointer(1, 2, GLES30.GL_FLOAT, false, 2 * 4, 0);
		GLES30.glEnableVertexAttribArray(1);
		if (lighting != null) {
			GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, VBO_normal);
			GLES30.glBufferData(GLES30.GL_ARRAY_BUFFER, normalBufferSize, normalBuffer, GLES30.GL_STATIC_DRAW);
			GLES30.glVertexAttribPointer(2, 3, GLES30.GL_FLOAT, false, 3 * 4, 0);
			GLES30.glEnableVertexAttribArray(2);
		}
		GLES30.glBindBuffer(GLES30.GL_ELEMENT_ARRAY_BUFFER, EBO);
		GLES30.glBufferData(GLES30.GL_ELEMENT_ARRAY_BUFFER, indexBufSize, indices, GLES30.GL_STATIC_DRAW);
		GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, 0);
		GLES30.glBindVertexArray(0);
		if (lighting != null) {
			lighting.bind();
			GLES30.glUniform1f(GLES30.glGetUniformLocation(p.programId, "useLighting"), 1f);
		} else {
			GLES30.glUniform1f(GLES30.glGetUniformLocation(p.programId, "useLighting"), 0f);
		}
	}
	public RenderUtils(Program program, Lighting li, GLSurfaceView gsv) {
		p = program;
		lighting = li;
		this.gsv = gsv;
	}
}