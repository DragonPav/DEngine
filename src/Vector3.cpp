#include "Vector3.h"
Vector3::Vector3(GLfloat x, GLfloat y, GLfloat z) {
	this->x = x;
	this->y = y;
	this->z = z;
}
Vector3::Vector3(GLint loc) {
	uniformL = loc;
}
void Vector3::uniform() {
	if (uniformL == -1) {
		throw std::runtime_error("uniform is -1");
	}
	glUniform3f(uniformL, x, y, z);
}
GLfloat Vector3::getX() {
	return x;
}
GLfloat Vector3::getY() {
	return y;
}
GLfloat Vector3::getZ() {
	return z;
}
GLint Vector3::getUniform() {
	return uniformL;
}
void Vector3::setX(GLfloat x) {
	this->x = x;
}
void Vector3::setY(GLfloat y) {
	this->y = y;
}
void Vector3::setZ(GLfloat z) {
	this->z = z;
}
void Vector3::setUniform(GLint unif) {
	if (unif >= -1) {
		uniformL = unif;
	}
	else {
		throw std::runtime_error("uniform less than -1 invalid");
	}
}