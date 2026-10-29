#pragma once
#include <iostream>
#include "Vector3.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#ifdef __ANDROID__
	#include <GLES3/gl32.h>
#else
	#include <GL/glew.h>
#endif
class Object3D;
/** @class Matrix4
 * @brief Used for matrix operations
 */
class Matrix4 {
public:
	glm::mat4 values{};
	GLint uniformL = -1;
	void uniform();
	void rotate(Vector3 axis, float angle, Object3D* obj, bool usePos);
	void translate(Vector3 pos);
	void scale(Vector3 size, Object3D* obj, bool usePos);
	Matrix4();
	Matrix4(GLint loc);
};