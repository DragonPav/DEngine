package com.dragonpav.dengine;

import android.opengl.GLES30;
import android.util.Log;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.util.ArrayList;

import glm_.mat4x4.Mat4;

public class StaticModel {
    public Values.Matrix4 model;
    public FloatBuffer vertexBuffer;
    public IntBuffer indexBuffer;
    public FloatBuffer texBuffer;
    public FloatBuffer normalBuffer;
    public Texture texture;
    public int vertBufSize, indexBufSize, texBufSize, normalBufSize;
    public ObjectBuffer b;
    Camera cam;
    RenderUtils renderUtils;
    public StaticModel(Camera camera, RenderUtils rend, ArrayList<Object3D> objects) {
        cam = camera;
        renderUtils = rend;
        texture = objects.get(0).texture;
        boolean usesLighting = rend.lighting != null;
        int indexOffset = 0;
        for (Object3D obj : objects) {
            vertBufSize += obj.vertexBufferSize;
            indexBufSize += obj.indexBufferSize;
            texBufSize += obj.texBufferSize;
            normalBufSize += obj.normalBufferSize;
        }
        vertexBuffer = ByteBuffer.allocateDirect(vertBufSize)
                .order(ByteOrder.nativeOrder())
                .asFloatBuffer();
        indexBuffer = ByteBuffer.allocateDirect(indexBufSize)
                .order(ByteOrder.nativeOrder())
                .asIntBuffer();
        texBuffer = ByteBuffer.allocateDirect(texBufSize)
                .order(ByteOrder.nativeOrder())
                .asFloatBuffer();
        if (usesLighting) {
            normalBuffer = ByteBuffer.allocateDirect(normalBufSize)
                    .order(ByteOrder.nativeOrder())
                    .asFloatBuffer();
        }
        for (Object3D obj : objects) {
            float[] verts = new float[obj.vertexBuffer.capacity()];
            obj.vertexBuffer.get(verts);
            vertexBuffer.put(verts);
            obj.vertexBuffer.position(0);
            for (int i = 0; i < obj.indexBuffer.capacity(); i++) {
                indexBuffer.put(obj.indexBuffer.get(i) + indexOffset);
            }
            float[] tex = new float[obj.texBuffer.capacity()];
            obj.texBuffer.get(tex);
            texBuffer.put(tex);
            obj.texBuffer.position(0);
            if (usesLighting) {
                float[] norms = new float[obj.normalBuffer.capacity()];
                obj.normalBuffer.get(norms);
                normalBuffer.put(norms);
                obj.normalBuffer.position(0);
            }
            indexOffset += obj.vertexBuffer.capacity() / 3;
        }
        vertexBuffer.position(0);
        indexBuffer.position(0);
        texBuffer.position(0);
        if (usesLighting) normalBuffer.position(0);
        model = new Values.Matrix4(GLES30.glGetUniformLocation(rend.p.programId, "model"));
        b = renderUtils.createStaticVAO(vertexBuffer, indexBuffer, texBuffer, normalBuffer, vertBufSize, indexBufSize, texBufSize, normalBufSize);
    }
    public void begin() {
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
        if (cam.rendUtils.lighting != null) {
            cam.rendUtils.lighting.bind();
        }
        if (texture.texSampleNum == -1) {
            Log.e("DEngine", "texture.texSampleNum is -1");
            return;
        }
        else {
            GLES30.glUniform1i(texture.texSampleNum, texture.glType % GLES30.GL_TEXTURE0);
        }
        GLES30.glDrawElements(GLES30.GL_TRIANGLES, indexBufSize / 4, GLES30.GL_UNSIGNED_INT, 0);
    }
    public void end() {
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, 0);
        GLES30.glBindVertexArray(0);
    }
    public void dispose() {
        end();
        GLES30.glDeleteVertexArrays(1, new int[] {b.VAO}, 0);
        ArrayList<Integer> buffersToDelete = new ArrayList<>();
        buffersToDelete.add(b.VBO);
        buffersToDelete.add(b.VBOtexture);
        buffersToDelete.add(b.EBO);
        if (cam.rendUtils.lighting != null) buffersToDelete.add(b.VBOnormal);
        int[] tmp = buffersToDelete.stream().mapToInt(Integer::intValue).toArray();
        GLES30.glDeleteBuffers(buffersToDelete.size(), tmp, 0);
        b = new ObjectBuffer();
    }
}
