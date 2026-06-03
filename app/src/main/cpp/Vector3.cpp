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
		__android_log_write(ANDROID_LOG_WARN, "DEngine", "uniform is -1");
		return;
	}
	glUniform3f(uniformL, x, y, z);
}