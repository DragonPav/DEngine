package com.dragonpav.dengine;

import android.opengl.GLES30;
import android.util.Log;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

public class Shader {
	public int shaderId;
	private String source;
	public Type type;
	public Shader(InputStream file, Type type) {
		this.type = type;
		try {
			BufferedReader br = new BufferedReader(new InputStreamReader(file));
			String line;
			StringBuilder builder = new StringBuilder();
			while ((line = br.readLine()) != null) {
				builder.append(line).append('\n');
			}
			source = builder.toString();
			file.close();
		} catch (IOException e) {
			e.fillInStackTrace();
		}
		switch (type) {
			case Vertex:
				shaderId = GLES30.glCreateShader(GLES30.GL_VERTEX_SHADER);
				break;
			case Fragment:
				shaderId = GLES30.glCreateShader(GLES30.GL_FRAGMENT_SHADER);
				break;
		}
		Log.i("DEngine", "Shader with type: " + type.name() + ", id: " + shaderId + " created.");
		GLES30.glShaderSource(shaderId, source);
	}
	public boolean compile() {
		GLES30.glCompileShader(shaderId);
		int[] status = new int[1];
		GLES30.glGetShaderiv(shaderId, GLES30.GL_COMPILE_STATUS, status, 0);
		if (status[0] == 1) {
			Log.i("DEngine", "Shader with id: " + shaderId + " successfully compiled.");
			return true;
		} else {
			String infoLog = GLES30.glGetShaderInfoLog(shaderId);
			Log.w("DEngine", "Failed compile shader (type: " + type.name() + ") Error log:");
			Log.w("DEngine", "------------------------");
			Log.w("DEngine", infoLog);
			delete();
			return false;
		}
	}
	public void delete() {
		GLES30.glDeleteShader(shaderId);
	}
	public enum Type {
		Vertex,
		Fragment
	}
}