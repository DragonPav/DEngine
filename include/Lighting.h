#pragma once
#include "Vector3.h"
#include "Program.h"
#include "Object3D.h"
#include <vector>
#ifdef __ANDROID__
	#include <GLES3/gl32.h>
#else
	#include <GL/glew.h>
#endif
class DirLight {
private:
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
	friend class Lighting;
public:
	DirLight() = delete;
	DirLight(Program* p);
	void setProgram(Program* prog);
	void setAmbient(Vector3 a);
	void setDiffuse(Vector3 d);
	void setSpecular(Vector3 s);
	void setDirection(Vector3 dir);
	Program* getProgram();
	Vector3& getAmbient();
	Vector3& getDiffuse();
	Vector3& getSpecular();
	Vector3& getDirection();
	void initShadows();
	void dispose();
	void bind();
};
class PointLight {
private:
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
public:
	void setProgram(Program* prog);
	void setAmbient(Vector3 a);
	void setDiffuse(Vector3 d);
	void setSpecular(Vector3 s);
	void setPosition(Vector3 dir);
	void setConstantLocation(GLint loc);
	void setLinearLocation(GLint loc);
	void setQuadraticLocation(GLint loc);
	Program* getProgram();
	Vector3& getAmbient();
	Vector3& getDiffuse();
	Vector3& getSpecular();
	Vector3& getPosition();
	GLint getConstantLocation();
	GLint getLinearLocation();
	GLint getQuadraticLocation();
	void bind();
	PointLight() = delete;
	PointLight(Program* p, GLint number);
};
class Lighting {
private:
	GLint shadowPassMatrixLoc = -1;
public:
	Vector3 cameraPos;
	DirLight dirLight;
	std::vector<PointLight*> pointLights;
	void bind();
	Lighting() = delete;
	Lighting(Vector3 camPos, Program* p);
	~Lighting();
	void dispose();
	void beginShadows(Program* shProg);
	void endShadows(int width, int height, Program* mainProg);
};