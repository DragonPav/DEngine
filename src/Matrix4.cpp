#include "Object3D.h"
#include "Matrix4.h"
void Matrix4::uniform() {
	if (uniformL == -1) {
		throw std::runtime_error("uniform is -1");
	}
	glUniformMatrix4fv(uniformL, 1, GL_FALSE, glm::value_ptr(values));
}
void Matrix4::rotate(Vector3 axis, float angle, Object3D* obj, bool usePos) {
	if (obj != nullptr && usePos) values = glm::translate(values, glm::vec3(-obj->getPosition().getX(), -obj->getPosition().getY(), -obj->getPosition().getZ()));
	values = glm::rotate(values, angle, glm::vec3(axis.getX(), axis.getY(), axis.getZ()));
	if (obj != nullptr && usePos) values = glm::translate(values, glm::vec3(obj->getPosition().getX(), obj->getPosition().getY(), obj->getPosition().getZ()));
};
void Matrix4::translate(Vector3 pos) {
	values = glm::translate(values, glm::vec3(pos.getX(), pos.getY(), pos.getZ()));
}
void Matrix4::scale(Vector3 size, Object3D* obj, bool usePos) {
	if (obj != nullptr && usePos) values = glm::translate(values, glm::vec3(-obj->getPosition().getX(), -obj->getPosition().getY(), -obj->getPosition().getZ()));
	values = glm::scale(values, glm::vec3(size.getX(), size.getY(), size.getZ()));
	if (obj != nullptr && usePos) values = glm::translate(values, glm::vec3(obj->getPosition().getX(), obj->getPosition().getY(), obj->getPosition().getZ()));
}
Matrix4::Matrix4() {
	values = glm::mat4(1.0f);
}
Matrix4::Matrix4(GLint loc) {
	uniformL = loc;
}