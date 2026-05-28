package com.dragonpav.dengine;

import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.opengl.GLES30;
import android.util.Log;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;

import glm_.mat4x4.Mat4;

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
	public ObjectBuffer b;
	public BoundingBox bounds;
	public int polygonMode = GLES30.GL_TRIANGLES;
	public void begin() {
		if (renderUtils.lighting != null) {
			renderUtils.lighting.bind();
		}
		model.values = new Mat4(1.0f);
		GLES30.glBindVertexArray(b.VAO);
	}
	public void render() {
		model.uniform();
		GLES30.glActiveTexture(texture.glType);
		GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, texture.textureId);
		if (cam.autoUpdate) {
			cam.update();
		}
		if (texture.texSampleNum != -1) {
			GLES30.glUniform1i(texture.texSampleNum, texture.glType % GLES30.GL_TEXTURE0);
		}
		GLES30.glDrawElements(polygonMode, indexBufferSize / 4, GLES30.GL_UNSIGNED_INT, 0);
		int err;
		while ((err = GLES30.glGetError()) != GLES30.GL_NO_ERROR) {
			Log.e("DEngine", String.format("OpenGL error occured: 0x%X", err));
		}
	}
	public void end() {
		GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, 0);
		GLES30.glBindVertexArray(0);
	}
	public void setup() {
		b = renderUtils.createStaticVAO(vertexBuffer, indexBuffer, texBuffer, normalBuffer,
				vertexBufferSize, indexBufferSize, texBufferSize, normalBufferSize);
	}
	public void dispose() {
		end();
		if (b.VAO != 0) {
			int[] vaoArray = new int[] {b.VAO};
			GLES30.glDeleteVertexArrays(1, vaoArray, 0);
			b.VAO = 0;
		}
		int count = 0;
		if (b.VBO != 0) count++;
		if (b.VBOtexture != 0) count++;
		if (b.VBOnormal != 0) count++;
		if (b.EBO != 0) count++;
		if (count > 0) {
			int[] buffersToDelete = new int[count];
			int index = 0;
			if (b.VBO != 0) {
				buffersToDelete[index++] = b.VBO;
			}
			if (b.VBOtexture != 0) {
				buffersToDelete[index++] = b.VBOtexture;
			}
			if (b.VBOnormal != 0) {
				buffersToDelete[index++] = b.VBOnormal;
			}
			if (b.EBO != 0) {
				buffersToDelete[index++] = b.EBO;
			}
			b = new ObjectBuffer();
			GLES30.glDeleteBuffers(count, buffersToDelete, 0);
		}
	}
	public Object3D(Camera camera, RenderUtils rend, Texture tex) {
		cam = camera;
		renderUtils = rend;
		texture = tex;
		model = new Values.Matrix4(GLES30.glGetUniformLocation(rend.p.programId, "model"));
	}
}