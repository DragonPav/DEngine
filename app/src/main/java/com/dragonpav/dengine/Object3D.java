package com.dragonpav.dengine;

import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.opengl.GLES30;
import android.opengl.Matrix;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;

public class Object3D {
	public Values.Vector3 position;
	public Values.Matrix4 model;
	private Camera cam;
	private RenderUtils renderUtils;
	public FloatBuffer vertexBuffer = null;
	public IntBuffer indexBuffer = null;
	public FloatBuffer texBuffer = null;
	public FloatBuffer normalBuffer = null;
	public int vertexBufferSize, indexBufferSize, texBufferSize, normalBufferSize;
	public Texture texture;
	public boolean wireframe = false;
	public void begin() {
		renderUtils.bindData(vertexBuffer, indexBuffer, texBuffer, vertexBufferSize, indexBufferSize, texBufferSize, normalBuffer, normalBufferSize);
		Matrix.setIdentityM(model.values, 0);
		GLES30.glBindVertexArray(renderUtils.VAO);
	}
	public void render() {
		model.uniform();
		GLES30.glActiveTexture(texture.glType);
		GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, texture.textureId);
		if (cam.autoUpdate) {
			cam.update();
		}
		if (texture.texSampleNum == -1) {
			throw new IllegalStateException("texture.texSampleNum is -1");
		} else {
			GLES30.glUniform1i(texture.texSampleNum, texture.glType % GLES30.GL_TEXTURE0);
		}
		GLES30.glDrawElements(GLES30.GL_TRIANGLES, indexBufferSize / 4, GLES30.GL_UNSIGNED_INT, 0);
	}
	public void end() {
		GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, 0);
		GLES30.glBindVertexArray(0);
	}
	public Object3D(Camera camera, RenderUtils rend, Texture tex) {
		cam = camera;
		renderUtils = rend;
		texture = tex;
		model = new Values.Matrix4(GLES30.glGetUniformLocation(rend.p.programId, "model"));
	}
}