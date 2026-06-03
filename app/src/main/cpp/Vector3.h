#pragma once
#include "Vector2.h"
class Vector3 {
public:
	GLfloat x = 0, y = 0, z = 0;
	GLint uniformL = -1;
	void uniform();
	Vector3() = default;
	Vector3(GLfloat x, GLfloat y, GLfloat z);
	Vector3(GLint loc);
};