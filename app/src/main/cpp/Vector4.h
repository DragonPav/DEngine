#pragma once
#include "Vector2.h"
class Vector4 {
public:
	GLfloat x = 0, y = 0, z = 0, w = 0;
	GLint uniformL = -1;
	void uniform();
	Vector4() = default;
	Vector4(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	Vector4(GLint loc);
};