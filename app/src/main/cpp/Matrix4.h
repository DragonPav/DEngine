#pragma once
#include <iostream>
#include "Vector3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <GLES3/gl32.h>
class Object3D;
class Matrix4 {
public:
	glm::mat4 values{};
	GLint uniformL = -1;
	void uniform();
	void rotate(const Vector3 &axis, float angle, Object3D* obj, bool usePos);
	void translate(const Vector3 &pos);
	void scale(const Vector3 &size, Object3D* obj, bool usePos);
	Matrix4();
	Matrix4(GLint loc);
};