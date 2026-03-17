package com.dragonpav.dengine;

import android.opengl.GLES30;

public class Lighting {
	public DirLight dirLight;
	public PointLight[] pointLights;
	public Values.Vector3 cameraPos;
	public void bind() {
		cameraPos.uniform();
		dirLight.bind();
		for (PointLight pointLight : pointLights) {
			pointLight.bind();
		}
	}
	public Lighting(Values.Vector3 camPos, Program p) {
		cameraPos = new Values.Vector3(camPos.x, camPos.y, camPos.z);
		cameraPos.uniform = GLES30.glGetUniformLocation(p.programId, "cameraPos");
		dirLight = new DirLight(p);
		pointLights = new PointLight[] {new PointLight(p, 0)};
	}
	public static class DirLight {
		public Values.Vector3 ambient;
		public Values.Vector3 diffuse;
		public Values.Vector3 specular;
		public Values.Vector3 direction;
		public void bind() {
			ambient.uniform();
			diffuse.uniform();
			specular.uniform();
			direction.uniform();
		}
		public DirLight(Program p) {
			ambient = new Values.Vector3(0.1f, 0.1f, 0.1f);
			ambient.uniform = GLES30.glGetUniformLocation(p.programId, "dirLight.ambient");
			diffuse = new Values.Vector3(0.6f, 0.6f, 0.6f);
			diffuse.uniform = GLES30.glGetUniformLocation(p.programId, "dirLight.diffuse");
			specular = new Values.Vector3(1f, 1f, 1f);
			specular.uniform = GLES30.glGetUniformLocation(p.programId, "dirLight.specular");
			direction = new Values.Vector3(0f, -5f, -2f);
			direction.uniform = GLES30.glGetUniformLocation(p.programId, "dirLight.direction");
		}
	}
	public static class PointLight {
		public Values.Vector3 ambient;
		public Values.Vector3 diffuse;
		public Values.Vector3 specular;
		public Values.Vector3 position;
		public float constant;
		public float linear;
		public float quadratic;
		public int constantLoc = -1;
		public int linearLoc = -1;
		public int quadraticLoc = -1;
		public void bind() {
			ambient.uniform();
			diffuse.uniform();
			specular.uniform();
			position.uniform();
			if (constantLoc != -1) {
				GLES30.glUniform1f(constantLoc, constant);
			}
			if (linearLoc != -1) {
				GLES30.glUniform1f(linearLoc, linear);
			}
			if (quadraticLoc != -1) {
				GLES30.glUniform1f(quadraticLoc, quadratic);
			}
		}
		public PointLight(Program p, int number) {
			ambient = new Values.Vector3(0.1f, 0.1f, 0.1f);
			ambient.uniform = GLES30.glGetUniformLocation(p.programId, "pointLights[" + number + "].ambient");
			diffuse = new Values.Vector3(0.6f, 0.6f, 0.6f);
			diffuse.uniform = GLES30.glGetUniformLocation(p.programId, "pointLights[" + number + "].diffuse");
			specular = new Values.Vector3(1f, 1f, 1f);
			specular.uniform = GLES30.glGetUniformLocation(p.programId, "pointLights[" + number + "].specular");
			position = new Values.Vector3(0, 0, 0);
			position.uniform = GLES30.glGetUniformLocation(p.programId, "pointLights[" + number + "].position");
			constant = 1.0f;
			linear = 0.09f;
			quadratic = 0.032f;
			constantLoc = GLES30.glGetUniformLocation(p.programId, "pointLights[" + number + "].constant");
			linearLoc = GLES30.glGetUniformLocation(p.programId, "pointLights[" + number + "].linear");
			quadraticLoc = GLES30.glGetUniformLocation(p.programId, "pointLights[" + number + "].quadratic");
		}
	}
}