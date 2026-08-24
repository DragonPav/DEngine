#include "Vector4.h"
Vector4::Vector4(GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}
Vector4::Vector4(GLint loc) {
	uniformL = loc;
}
void Vector4::uniform() {
	if (uniformL == -1) {
		throw std::runtime_error("uniform is -1");
	}
	glUniform4f(uniformL, x, y, z, w);
}
GLfloat Vector4::getX() {
	return x;
}
GLfloat Vector4::getY() {
	return y;
}
GLfloat Vector4::getZ() {
	return z;
}
GLfloat Vector4::getW() {
	return w;
}
GLint Vector4::getUniform() {
	return uniformL;
}
void Vector4::setX(GLfloat x) {
	this->x = x;
}
void Vector4::setY(GLfloat y) {
	this->y = y;
}
void Vector4::setZ(GLfloat z) {
	this->z = z;
}
void Vector4::setW(GLfloat w) {
	this->w = w;
}
void Vector4::setUniform(GLint unif) {
	if (unif >= -1) {
		uniformL = unif;
	}
	else {
		throw std::runtime_error("uniform less than -1 invalid");
	}
}