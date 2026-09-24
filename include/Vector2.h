#pragma once
#include <iostream>
#ifdef __ANDROID__
	#include <GLES3/gl32.h>
#else
	#include <GL/glew.h>
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