package com.dragonpav.dengine;

import android.opengl.GLES30;

public class Lighting {
	public float ambientStrength = 0.2f;
	public float specularStrength = 0.5f;
	public Values.Vector3 lightPos;
	public Values.Vector3 lightColor;
	public Values.Vector3 cameraPos;
	public int ambientStrengthLoc;
	public int specularStrengthLoc;
	public void bind() {
		cameraPos.uniform();
		lightPos.uniform();
		lightColor.uniform();
		if (ambientStrengthLoc != -1) {
			GLES30.glUniform1f(ambientStrengthLoc, ambientStrength);
		}
		if (specularStrengthLoc != -1) {
			GLES30.glUniform1f(specularStrengthLoc, specularStrength);
		}
	}
	public Lighting(Values.Vector3 camPos, Program p) {
		cameraPos = new Values.Vector3(camPos.x, camPos.y, camPos.z);
		cameraPos.uniform = GLES30.glGetUniformLocation(p.programId, "cameraPos");
		lightPos = new Values.Vector3(0, 5, 3);
		lightPos.uniform = GLES30.glGetUniformLocation(p.programId, "lightPos");
		lightColor = new Values.Vector3(1, 1, 1);
		lightColor.uniform = GLES30.glGetUniformLocation(p.programId, "lightColor");
		ambientStrengthLoc = GLES30.glGetUniformLocation(p.programId, "ambientStrength");
		specularStrengthLoc = GLES30.glGetUniformLocation(p.programId, "specularStrength");
	}
}