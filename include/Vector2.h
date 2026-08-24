#pragma once
#include <iostream>
#ifdef _WIN32
	#include <GL/glew.h>
#else
	#include <GLES3/gl32.h>
#endif
#include <glm/vec2.hpp>
class Vector2 {
private:
	GLfloat x = 0, y = 0;
	GLint uniformL = -1;
public:
	void uniform();
	GLfloat getX();
	GLfloat getY();
	GLint getUniform();
	void setX(GLfloat x);
	void setY(GLfloat y);
	void setUniform(GLint unif);
	Vector2() = default;
	Vector2(GLfloat x, GLfloat y);
	Vector2(GLint loc);
};