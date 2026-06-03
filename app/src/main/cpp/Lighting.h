#pragma once
#include "Vector3.h"
#include "Program.h"
#include "Object3D.h"
#include <vector>
#include <GLES3/gl32.h>
class DirLight {
public:
	Program* p;
	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;
	Vector3 direction;
	GLuint depthFBO;
	GLuint depthMap;
	const unsigned int SHADOW_SIZE = 2048;
	Matrix4 lightSpaceMatrix;
	GLint shadowMapLoc = -1;
	DirLight() = delete;
	DirLight(Program *p);
	void initShadows();
	void dispose();
	void bind();
};
class PointLight {
public:
	Program* p;
	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;
	Vector3 position;
	GLfloat constant = 1;
	GLfloat linear = 0.09f;
	GLfloat quadratic = 0.032f;
	GLint constantLoc = -1;
	GLint linearLoc = -1;
	GLint quadraticLoc = -1;
	void bind();
	PointLight() = delete;
	PointLight(Program* p, GLint number);
};
class Lighting {
public:
	Vector3 cameraPos;
	DirLight dirLight;
	std::vector<PointLight> pointLights;
	void bind();
	Lighting() = delete;
	Lighting(Vector3 camPos, Program* p);
	void beginShadows(Program* shProg);
	void endShadows(int width, int height, Program* mainProg);
};