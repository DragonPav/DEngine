#pragma once
#include <iostream>
#ifdef _WIN32
	#include <GL/glew.h>
#else
	#include <GLES3/gl32.h>
#endif
class Vector3 {
private:
	GLfloat x = 0, y = 0, z = 0;
	GLint uniformL = -1;
public:
	void uniform();
	GLfloat getX();
	GLfloat getY();
	GLfloat getZ();
	GLint getUniform();
	void setX(GLfloat x);
	void setY(GLfloat y);
	void setZ(GLfloat z);
	void setUniform(GLint unif);
	Vector3() = default;
	Vector3(GLfloat x, GLfloat y, GLfloat z);
	Vector3(GLint loc);
};