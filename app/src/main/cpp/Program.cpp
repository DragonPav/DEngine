#include "Program.h"
Program::Program(Shader* vertex, Shader* fragment) {
	v = vertex;
	f = fragment;
	if (vertex->type != Shader::Type::Vertex) {
		__android_log_write(ANDROID_LOG_ERROR, "DEngine", "vShader must be Vertex");
	}
	if (fragment->type != Shader::Type::Fragment) {
		__android_log_write(ANDROID_LOG_ERROR, "DEngine", "fShader must be Fragment");
	}
	programId = glCreateProgram();
	glAttachShader(programId, vertex->shaderId);
	glAttachShader(programId, fragment->shaderId);
	std::string str = "Program with id: " + std::to_string(programId) + " created.";
	__android_log_write(ANDROID_LOG_INFO, "DEngine", str.c_str());
}
void Program::deleteProgram() {
	glDeleteProgram(programId);
}
bool Program::link() {
	glLinkProgram(programId);
	GLint status;
	glGetProgramiv(programId, GL_LINK_STATUS, &status);
	std::string str;
	if (status == GL_TRUE) {
		v->deleteShader();
		f->deleteShader();
		str = "Program with id: " + std::to_string(programId) + " successfully linked.";
		__android_log_write(ANDROID_LOG_INFO, "DEngine", str.c_str());
		return true;
	}
	else {
		GLchar infoLog[512];
		glGetProgramInfoLog(programId, 512, NULL, infoLog);
		str = std::string("Failed to link program: ") + infoLog;
		__android_log_write(ANDROID_LOG_ERROR, "DEngine", str.c_str());
		deleteProgram();
		return false;
	}
}