package com.dragonpav.dengine;

import android.opengl.GLES30;

import glm_.mat4x4.Mat4;

public class Values {
	public static class Vector2 {
		public float x, y;
		public int uniform = -1;
		public void uniform() {
			if (uniform == -1) {
				throw new IllegalStateException("uniform is -1");
			}
			GLES30.glUniform2f(uniform, x, y);
		}
		@Override
		public String toString() {
			return "X: " + x + " Y: " + y;
		}
		public Vector2() {}
		public Vector2(float x, float y) {
			this.x = x;
			this.y = y;
		}
		public Vector2(int loc) {
			uniform = loc;
		}
	}
	public static class Vector3 {
		public float x = 0, y = 0, z = 0;
		public float[] rgb;
		public int uniform = -1;
		public void uniform() {
			if (uniform == -1) {
				throw new IllegalStateException("uniform is -1");
			}
			if (rgb != null) {
				GLES30.glUniform3fv(uniform, 1, rgb, 0);
			} else {
				GLES30.glUniform3f(uniform, x, y, z);
			}
		}
		@Override
		public String toString() {
			return "X: " + x + " Y: " + y + " Z: " + z;
		}
		public Vector3() {}
		public Vector3(float x, float y, float z) {
			this.x = x;
			this.y = y;
			this.z = z;
		}
		public Vector3(int loc) {
			uniform = loc;
		}
	}
	public static class Vector4 {
		public float x, y, z, w;
		public int uniform = -1;
		public void uniform() {
			if (uniform == -1) {
				throw new IllegalStateException("uniform is -1");
			}
			GLES30.glUniform4f(uniform, x, y, z, w);
		}
		@Override
		public String toString() {
			return "X: " + x + " Y: " + y + " Z: " + z + " W: " + w;
		}
		public Vector4() {}
		public Vector4(float x, float y, float z, float w) {
			this.x = x;
			this.y = y;
			this.z = z;
			this.w = w;
		}
		public Vector4(int loc) {
			uniform = loc;
		}
	}
	public static class Matrix4 {
		public int uniform = -1;
		public Mat4 values;
		public void uniform() {
			if (uniform == -1) {
				throw new IllegalStateException("uniform is -1");
			}
			GLES30.glUniformMatrix4fv(uniform, 1, false, values.array, 0);
		}
		public void rotate(Vector3 axis, float angle, Object3D obj) {
			if (obj != null && obj.position != null) values.translate(obj.position.x, obj.position.y, obj.position.z);
			values.rotate(angle, axis.x, axis.y, axis.z);
			if (obj != null && obj.position != null) values.translate(-obj.position.x, -obj.position.y, -obj.position.z);
		}
		public void translate(Vector3 pos) {
			values.translate(pos.x, pos.y, pos.z);
		}
		public void scale(Vector3 size, Object3D obj) {
			if (obj != null && obj.position != null) values.translate(obj.position.x, obj.position.y, obj.position.z);
			values.scale(size.x, size.y, size.z);
			if (obj != null && obj.position != null) values.translate(-obj.position.x, -obj.position.y, -obj.position.z);
		}
		@Override
		public String toString() {
			StringBuilder builder = new StringBuilder();
			builder.append("[ ");
			int count = 0;
			for (int i = 0; i < values.size(); i++) {
				if (i != values.size() - 1) {
					builder.append(values.get(i)).append(", ");
				} else {
					builder.append(values.get(i));
				}
				if (count == 4) {
					builder.append('\n');
					count = 0;
				} else {
					count++;
				}
			}
			builder.append(']');
			return builder.toString();
		}
		public Matrix4() {
			values = new Mat4();
		}
		public Matrix4(int loc) {
			values = new Mat4();
			uniform = loc;
		}
	}
}