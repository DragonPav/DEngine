package com.dragonpav.dengine;

import android.opengl.GLES30;
import android.util.Log;

public class Program {
	public int programId;
	private Shader v, f;
	public Program(Shader vShader, Shader fShader) {
		v = vShader;
		f = fShader;
		if (vShader.type != Shader.Type.Vertex) {
			throw new IllegalArgumentException("vShader must be Vertex");
		}
		if (fShader.type != Shader.Type.Fragment) {
			throw new IllegalArgumentException("fShader must be Fragment");
		}
		programId = GLES30.glCreateProgram();
		GLES30.glAttachShader(programId, vShader.shaderId);
		GLES30.glAttachShader(programId, fShader.shaderId);
		Log.i("DEngine", "Program with id: " + programId + " created.");
	}
	public boolean link() {
		GLES30.glLinkProgram(programId);
		int[] status = new int[1];
		GLES30.glGetProgramiv(programId, GLES30.GL_LINK_STATUS, status, 0);
		if (status[0] == 1) {
			v.delete();
			f.delete();
			Log.i("DEngine", "Program (id: " + programId + ") successfully linked.");
			return true;
		} else {
			String infoLog = GLES30.glGetProgramInfoLog(programId);
			Log.e("DEngine", "Failed to link program: " + infoLog);
			delete();
			return false;
		}
	}
	public void delete() {
		GLES30.glDeleteProgram(programId);
	}
}