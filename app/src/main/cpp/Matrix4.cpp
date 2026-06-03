#include "Object3D.h"
#include "Matrix4.h"
void Matrix4::uniform() {
	if (uniformL == -1) {
		__android_log_write(ANDROID_LOG_WARN, "DEngine", "uniform is -1");
		return;
	}
	glUniformMatrix4fv(uniformL, 1, GL_FALSE, glm::value_ptr(values));
}
void Matrix4::rotate(const Vector3 &axis, float angle, Object3D* obj, bool usePos) {
	if (obj != nullptr && usePos) values = glm::translate(values, glm::vec3(-obj->position.x, -obj->position.y, -obj->position.z));
	values = glm::rotate(values, angle, glm::vec3(axis.x, axis.y, axis.z));
	if (obj != nullptr && usePos) values = glm::translate(values, glm::vec3(obj->position.x, obj->position.y, obj->position.z));
};
void Matrix4::translate(const Vector3 &pos) {
	values = glm::translate(values, glm::vec3(pos.x, pos.y, pos.z));
}
void Matrix4::scale(const Vector3 &size, Object3D* obj, bool usePos) {
	if (obj != nullptr && usePos) values = glm::translate(values, glm::vec3(-obj->position.x, -obj->position.y, -obj->position.z));
	values = glm::scale(values, glm::vec3(size.x, size.y, size.z));
	if (obj != nullptr && usePos) values = glm::translate(values, glm::vec3(obj->position.x, obj->position.y, obj->position.z));
}
Matrix4::Matrix4() {
	values = glm::mat4(1.0f);
}
Matrix4::Matrix4(GLint loc) {
	uniformL = loc;
}