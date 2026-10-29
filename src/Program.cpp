#include "Program.h"
#include "Logger.h"
Program::Program(Shader* vertex, Shader* fragment) {
	v = vertex;
	f = fragment;
	if (vertex->getType() != Shader::Type::Vertex) {
		throw std::runtime_error("vShader must be Vertex");
	}
	if (fragment->getType() != Shader::Type::Fragment) {
		throw std::runtime_error("fShader must be Fragment");
	}
	programId = glCreateProgram();
	glAttachShader(programId, vertex->getId());
	glAttachShader(programId, fragment->getId());
	Logger::print(Logger::Info, "[Program]: Program with id: " + std::to_string(programId) + " created.");
}
Program::~Program() {
	deleteProgram();
}
void Program::deleteProgram() {
	glDeleteProgram(programId);
}
bool Program::link() {
	glLinkProgram(programId);
	GLint status;
	glGetProgramiv(programId, GL_LINK_STATUS, &status);
	if (status == GL_TRUE) {
		v->deleteShader();
		f->deleteShader();
		Logger::print(Logger::Info, "[Program::link()]: Program with id: " + std::to_string(programId) + " successfully linked.");
		return true;
	} else {
		GLchar infoLog[512] = {0};
		glGetProgramInfoLog(programId, sizeof(infoLog), nullptr, infoLog);
		std::string str = "[Program::link()]: Failed to link program with id: " + std::to_string(programId) + " Error log:";
		Logger::print(Logger::Error, str);
		Logger::print(Logger::Error, "-------------------------");
		Logger::print(Logger::Error, infoLog);
		deleteProgram();
		return false;
	}
}
GLuint Program::getId() {
	return programId;
}