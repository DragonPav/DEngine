package com.dragonpav.dengine;

import android.opengl.GLES30;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.util.ArrayList;

public class ObjectCreator {
	public static Object3D createCustom(float[] verts, int[] indxs, float[] texCoords, float[] normals, Camera camera, Texture tex) {
		Object3D ret = new Object3D(camera, camera.rendUtils, tex);
		ret.vertexBuffer = ByteBuffer.allocateDirect(verts.length * 4)
			.order(ByteOrder.nativeOrder())
			.asFloatBuffer();
		ret.vertexBuffer.put(verts);
		ret.vertexBuffer.position(0);
		ret.indexBuffer = ByteBuffer.allocateDirect(indxs.length * 4)
			.order(ByteOrder.nativeOrder())
			.asIntBuffer();
		ret.indexBuffer.put(indxs);
		ret.indexBuffer.position(0);
		ret.texBuffer = ByteBuffer.allocateDirect(texCoords.length * 4)
			.order(ByteOrder.nativeOrder())
			.asFloatBuffer();
		ret.texBuffer.put(texCoords);
		ret.texBuffer.position(0);
		if (camera.rendUtils.lighting != null) {
			ret.normalBuffer = ByteBuffer.allocateDirect(normals.length * 4)
				.order(ByteOrder.nativeOrder())
				.asFloatBuffer();
			ret.normalBuffer.put(normals);
			ret.normalBuffer.position(0);
		}
		ret.vertexBufferSize = verts.length * Float.BYTES;
		ret.indexBufferSize = indxs.length * Integer.BYTES;
		ret.texBufferSize = texCoords.length * Float.BYTES;
		ret.normalBufferSize = normals.length * Float.BYTES;
		ret.setup();
		return ret;
	}
	public static Object3D createBox(Values.Vector3 pos, Values.Vector3 size, Camera camera, Texture tex) {
		Object3D ret = new Object3D(camera, camera.rendUtils, tex);
		float halfWidth = size.x / 2;
		float halfHeight = size.y / 2;
		float halfDepth = size.z / 2;
		float[] vertices = {
			//back
			pos.x - halfWidth, pos.y - halfHeight, pos.z + halfDepth,
			pos.x + halfWidth, pos.y - halfHeight, pos.z + halfDepth,
			pos.x - halfWidth, pos.y + halfHeight, pos.z + halfDepth,
			pos.x + halfWidth, pos.y + halfHeight, pos.z + halfDepth,
			//left
			pos.x - halfWidth, pos.y - halfHeight, pos.z - halfDepth,
			pos.x - halfWidth, pos.y - halfHeight, pos.z + halfDepth,
			pos.x - halfWidth, pos.y + halfHeight, pos.z - halfDepth,
			pos.x - halfWidth, pos.y + halfHeight, pos.z + halfDepth,
			//front
			pos.x - halfWidth, pos.y - halfHeight, pos.z - halfDepth,
			pos.x + halfWidth, pos.y - halfHeight, pos.z - halfDepth,
			pos.x - halfWidth, pos.y + halfHeight, pos.z - halfDepth,
			pos.x + halfWidth, pos.y + halfHeight, pos.z - halfDepth,
			//right
			pos.x + halfWidth, pos.y - halfHeight, pos.z + halfDepth,
			pos.x + halfWidth, pos.y - halfHeight, pos.z - halfDepth,
			pos.x + halfWidth, pos.y + halfHeight, pos.z + halfDepth,
			pos.x + halfWidth, pos.y + halfHeight, pos.z - halfDepth,
			//top
			pos.x - halfWidth, pos.y + halfHeight, pos.z + halfDepth,
			pos.x + halfWidth, pos.y + halfHeight, pos.z + halfDepth,
			pos.x - halfWidth, pos.y + halfHeight, pos.z - halfDepth,
			pos.x + halfWidth, pos.y + halfHeight, pos.z - halfDepth,
			//bottom
			pos.x - halfWidth, pos.y - halfHeight, pos.z + halfDepth,
			pos.x + halfWidth, pos.y - halfHeight, pos.z + halfDepth,
			pos.x - halfWidth, pos.y - halfHeight, pos.z - halfDepth,
			pos.x + halfWidth, pos.y - halfHeight, pos.z - halfDepth
		};
		int[] indices = {
			//back
			0, 1, 2,
			2, 3, 1,
			//left
			4, 5, 6,
			6, 7, 5,
			//front
			8, 9, 10,
			10, 11, 9,
			//right
			12, 13, 14,
			14, 15, 13,
			//top
			16, 17, 18,
			18, 19, 17,
			//bottom
			20, 21, 22,
			22, 23, 21
		};
		float[] texCoords = new float[] {
			//back
			0, 1,
			1, 1,
			0, 0,
			1, 0,
			//left
			0, 1,
			1, 1,
			0, 0,
			1, 0,
			//front
			1, 1,
			0, 1,
			1, 0,
			0, 0,
			//right
			0, 1,
			1, 1,
			0, 0,
			1, 0,
			//top
			0, 1,
			1, 1,
			0, 0,
			1, 0,
			//bottom
			1, 1,
			0, 1,
			1, 0,
			0, 0
		};
		float[] normals = new float[0];
		if (camera.rendUtils.lighting != null) {
			normals = new float[] {
				//back
				0, 0, 1,
				0, 0, 1,
				0, 0, 1,
				0, 0, 1,
				//left
				-1, 0, 0,
				-1, 0, 0,
				-1, 0, 0,
				-1, 0, 0,
				//front
				0, 0, -1,
				0, 0, -1,
				0, 0, -1,
				0, 0, -1,
				//right
				1, 0, 0,
				1, 0, 0,
				1, 0, 0,
				1, 0, 0,
				//top
				0, 1, 0,
				0, 1, 0,
				0, 1, 0,
				0, 1, 0,
				//bottom
				0, -1, 0,
				0, -1, 0,
				0, -1, 0,
				0, -1, 0
			};
		}
		ret.position = pos;
		ret.vertexBuffer = ByteBuffer.allocateDirect(vertices.length * 4)
			.order(ByteOrder.nativeOrder())
			.asFloatBuffer();
		ret.vertexBuffer.put(vertices);
		ret.vertexBuffer.position(0);
		ret.indexBuffer = ByteBuffer.allocateDirect(indices.length * 4)
			.order(ByteOrder.nativeOrder())
			.asIntBuffer();
		ret.indexBuffer.put(indices);
		ret.indexBuffer.position(0);
		ret.texBuffer = ByteBuffer.allocateDirect(texCoords.length * 4)
			.order(ByteOrder.nativeOrder())
			.asFloatBuffer();
		ret.texBuffer.put(texCoords);
		ret.texBuffer.position(0);
		if (camera.rendUtils.lighting != null) {
			ret.normalBuffer = ByteBuffer.allocateDirect(normals.length * 4)
				.order(ByteOrder.nativeOrder())
				.asFloatBuffer();
			ret.normalBuffer.put(normals);
			ret.normalBuffer.position(0);
		}
		ret.vertexBufferSize = vertices.length * 4;
		ret.indexBufferSize = indices.length * 4;
		ret.texBufferSize = texCoords.length * 4;
		ret.normalBufferSize = normals.length * 4;
		ret.setup();
		return ret;
	}
	public static Object3D createQuad(Values.Vector3 pos, Values.Vector2 size, Camera camera, Texture tex) {
		Object3D ret = new Object3D(camera, camera.rendUtils, tex);
		float halfWidth = size.x / 2;
		float halfHeight = size.y / 2;
		float[] vertices = {
			pos.x - halfWidth, pos.y - halfHeight, pos.z,
			pos.x + halfWidth, pos.y - halfHeight, pos.z,
			pos.x - halfWidth, pos.y + halfHeight, pos.z,
			pos.x + halfWidth, pos.y + halfHeight, pos.z
		};
		int[] indices = {
			0, 1, 2,
			2, 3, 1
		};
		float[] texCoords = {
			0, 1,
			1, 1,
			0, 0,
			1, 0
		};
		float[] normals = new float[0];
		if (camera.rendUtils.lighting != null) {
			normals = new float[] {
				0, 0, 1,
				0, 0, 1,
				0, 0, 1,
				0, 0, 1
			};
		}
		ret.position = pos;
		ret.vertexBuffer = ByteBuffer.allocateDirect(vertices.length * Float.BYTES)
				.order(ByteOrder.nativeOrder())
				.asFloatBuffer();
		ret.vertexBuffer.put(vertices);
		ret.vertexBuffer.position(0);
		ret.indexBuffer = ByteBuffer.allocateDirect(indices.length * Integer.BYTES)
				.order(ByteOrder.nativeOrder())
				.asIntBuffer();
		ret.indexBuffer.put(indices);
		ret.indexBuffer.position(0);
		ret.texBuffer = ByteBuffer.allocateDirect(texCoords.length * Float.BYTES)
				.order(ByteOrder.nativeOrder())
				.asFloatBuffer();
		ret.texBuffer.put(texCoords);
		ret.texBuffer.position(0);
		if (camera.rendUtils.lighting != null) {
			ret.normalBuffer = ByteBuffer.allocateDirect(normals.length * Float.BYTES)
					.order(ByteOrder.nativeOrder())
					.asFloatBuffer();
			ret.normalBuffer.put(normals);
			ret.normalBuffer.position(0);
		}
		ret.vertexBufferSize = vertices.length * Float.BYTES;
		ret.indexBufferSize = indices.length * Integer.BYTES;
		ret.texBufferSize = texCoords.length * Float.BYTES;
		ret.normalBufferSize = normals.length * Float.BYTES;
		ret.setup();
		return ret;
	}
	public static Object3D createSurface(Values.Vector3 pos, Values.Vector2 size, Camera camera, Texture texture) {
		Object3D ret = new Object3D(camera, camera.rendUtils, texture);
		float halfWidth = size.x / 2;
		float halfDepth = size.y / 2;
		float[] vertices = {
			pos.x - halfWidth, pos.y, pos.z + halfDepth,
			pos.x + halfWidth, pos.y, pos.z + halfDepth,
			pos.x - halfWidth, pos.y, pos.z - halfDepth,
			pos.x + halfWidth, pos.y, pos.z - halfDepth
		};
		int[] indices = {
			0, 1, 2,
			2, 3, 1
		};
		float[] texCoords = {
			0, 1,
			1, 1,
			0, 0,
			1, 0
		};
		float[] normals = new float[0];
		if (camera.rendUtils.lighting != null) {
			normals = new float[] {
				0, 1, 0,
				0, 1, 0,
				0, 1, 0,
				0, 1, 0
			};
		}
		ret.position = pos;
		ret.vertexBuffer = ByteBuffer.allocateDirect(vertices.length * Float.BYTES)
				.order(ByteOrder.nativeOrder())
				.asFloatBuffer();
		ret.vertexBuffer.put(vertices);
		ret.vertexBuffer.position(0);
		ret.indexBuffer = ByteBuffer.allocateDirect(indices.length * Integer.BYTES)
				.order(ByteOrder.nativeOrder())
				.asIntBuffer();
		ret.indexBuffer.put(indices);
		ret.indexBuffer.position(0);
		ret.texBuffer = ByteBuffer.allocateDirect(texCoords.length * Float.BYTES)
				.order(ByteOrder.nativeOrder())
				.asFloatBuffer();
		ret.texBuffer.put(texCoords);
		ret.texBuffer.position(0);
		if (camera.rendUtils.lighting != null) {
			ret.normalBuffer = ByteBuffer.allocateDirect(normals.length * Float.BYTES)
					.order(ByteOrder.nativeOrder())
					.asFloatBuffer();
			ret.normalBuffer.put(normals);
			ret.normalBuffer.position(0);
		}
		ret.vertexBufferSize = vertices.length * Float.BYTES;
		ret.indexBufferSize = indices.length * Integer.BYTES;
		ret.texBufferSize = texCoords.length * Float.BYTES;
		ret.normalBufferSize = normals.length * Float.BYTES;
		ret.setup();
		return ret;
	}
	public static Object3D createSphere(Values.Vector3 pos, float radius, int sectorCount, int stackCount, Camera cam, Texture texture) {
		Object3D ret = new Object3D(cam, cam.rendUtils, texture);
		ArrayList<Float> vertices = new ArrayList<>();
		ArrayList<Integer> indices = new ArrayList<>();
		ArrayList<Float> texCoords = new ArrayList<>();
		ArrayList<Float> normals = new ArrayList<>();
		float x, y, z, xy;
		float nx, ny, nz, lengthInv = 1.0f / radius;
		float s, t;
		
		float sectorStep = 2 * (float) Math.PI / sectorCount;
		float stackStep = (float) Math.PI / stackCount;
		float sectorAngle, stackAngle;
		for (int i = 0; i <= stackCount; i++) {
			stackAngle = (float) Math.PI / 2 - i * stackStep;
			xy = radius * (float) Math.cos(stackAngle);
			z = radius * (float) Math.sin(stackAngle);
			for (int j = 0; j <= sectorCount; j++) {
				sectorAngle = j * sectorStep;
				x = xy * (float) Math.cos(sectorAngle);
				y = xy * (float) Math.sin(sectorAngle);
				vertices.add(x + pos.x);
				vertices.add(y + pos.y);
				vertices.add(z + pos.z);
				nx = x * lengthInv;
				ny = y * lengthInv;
				nz = z * lengthInv;
				normals.add(nx);
				normals.add(ny);
				normals.add(nz);
				s = (float) j / sectorCount;
				t = (float) i / stackCount;
				texCoords.add(s);
				texCoords.add(t);
			}
		}
		int k1, k2;
		for (int i = 0; i < stackCount; i++) {
			k1 = i * (sectorCount + 1);
			k2 = k1 + sectorCount + 1;
			for (int j = 0; j < sectorCount; j++, k1++, k2++) {
				if (i != 0) {
					indices.add(k1);
					indices.add(k2);
					indices.add(k1 + 1);
				}
				if (i != (stackCount - 1)) {
					indices.add(k1 + 1);
					indices.add(k2);
					indices.add(k2 + 1);
				}
			}
		}
		ret.position = pos;
		float[] verticesArr = new float[vertices.size()];
		int[] indicesArr = new int[indices.size()];
		float[] texCoordsArr = new float[texCoords.size()];
		float[] normalsArr = new float[normals.size()];
		for (int i = 0; i < vertices.size(); i++) {
			verticesArr[i] = vertices.get(i);
		}
		for (int i = 0; i < indices.size(); i++) {
			indicesArr[i] = indices.get(i);
		}
		for (int i = 0; i < texCoords.size(); i++) {
			texCoordsArr[i] = texCoords.get(i);
		}
		for (int i = 0; i < normals.size(); i++) {
			normalsArr[i] = normals.get(i);
		}
		vertices.clear();
		indices.clear();
		texCoords.clear();
		normals.clear();
		ret.vertexBuffer = ByteBuffer.allocateDirect(verticesArr.length * Float.BYTES)
			.order(ByteOrder.nativeOrder())
			.asFloatBuffer();
		ret.vertexBuffer.put(verticesArr);
		ret.vertexBuffer.position(0);
		ret.indexBuffer = ByteBuffer.allocateDirect(indicesArr.length * Integer.BYTES)
			.order(ByteOrder.nativeOrder())
			.asIntBuffer();
		ret.indexBuffer.put(indicesArr);
		ret.indexBuffer.position(0);
		ret.texBuffer = ByteBuffer.allocateDirect(texCoordsArr.length * Float.BYTES)
			.order(ByteOrder.nativeOrder())
			.asFloatBuffer();
		ret.texBuffer.put(texCoordsArr);
		ret.texBuffer.position(0);
		if (cam.rendUtils.lighting != null) {
			ret.normalBuffer = ByteBuffer.allocateDirect(normalsArr.length * Float.BYTES)
				.order(ByteOrder.nativeOrder())
				.asFloatBuffer();
			ret.normalBuffer.put(normalsArr);
			ret.normalBuffer.position(0);
		}
		ret.vertexBufferSize = verticesArr.length * Float.BYTES;
		ret.indexBufferSize = indicesArr.length * Integer.BYTES;
		ret.texBufferSize = texCoordsArr.length * Float.BYTES;
		ret.normalBufferSize = normalsArr.length * Float.BYTES;
		ret.setup();
		return ret;
	}
}