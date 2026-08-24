#pragma once
#include <iostream>
#ifdef _WIN32
	#include <GL/glew.h>
#else
	#include <GLES3/gl32.h>
#endif
class Vector4 {
public:
	GLfloat x = 0, y = 0, z = 0, w = 0;
	GLint uniformL = -1;
	void uniform();
	GLfloat getX();
	GLfloat getY();
	GLfloat getZ();
	GLfloat getW();
	GLint getUniform();
	void setX(GLfloat x);
	void setY(GLfloat y);
	void setZ(GLfloat z);
	void setW(GLfloat w);
	void setUniform(GLint unif);
	Vector4() = default;
	Vector4(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	Vector4(GLint loc);
};