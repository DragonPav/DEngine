#pragma once
#include <iostream>
#include <android/log.h>
#include <GLES3/gl32.h>
class Vector2 {
public:
	GLfloat x = 0, y = 0;
	GLint uniformL = -1;
	void uniform();
	Vector2() = default;
	Vector2(GLfloat x, GLfloat y);
	Vector2(GLint loc);
};