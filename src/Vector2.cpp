#include "Vector2.h"
#include "Logger.h"
Vector2::Vector2(GLfloat x, GLfloat y) {
	this->x = x;
	this->y = y;
}
Vector2::Vector2(GLint loc) {
	uniformL = loc;
}
void Vector2::uniform() {
	if (uniformL == -1) {
		Logger::print(Logger::Error, "[Vector2]: uniform is -1");
		return;
	}
	glUniform2f(uniformL, x, y);
}
GLfloat Vector2::getX() {
	return x;
}
GLfloat Vector2::getY() {
	return y;
}
GLint Vector2::getUniform() {
	return uniformL;
}
void Vector2::setX(GLfloat x) {
	this->x = x;
}
void Vector2::setY(GLfloat y) {
	this->y = y;
}
void Vector2::setUniform(GLint unif) {
	if (unif >= -1) {
		uniformL = unif;
	} else {
		Logger::print(Logger::Error, "[Vector2]: uniform less than -1 invalid");
		return;
	}
}