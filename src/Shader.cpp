#include "Shader.h"
#include "Logger.h"
#ifdef __ANDROID__
	Shader::Shader(AAsset *file, Type type) {
		if (file == nullptr) {
			std::string str = "Failed to open a file! in Shader";
			Logger::print(Logger::Error, str);
			return;
		}
		const char* buffer = (const char*) AAsset_getBuffer(file);
		off_t length = AAsset_getLength(file);
		std::string content(buffer, length);
		std::istringstream fileStream(content);
		AAsset_close(file);
		std::string source;
		std::string line;
		while (std::getline(fileStream, line)) {
			source += line + "\n";
		}
		std::string name;
		switch (type) {
		case Vertex:
			shaderId = glCreateShader(GL_VERTEX_SHADER);
			name = "Vertex";
			break;
		case Fragment:
			shaderId = glCreateShader(GL_FRAGMENT_SHADER);
			name = "Fragment";
			break;
		}
		std::string str = "Shader with type: " + name + ", id: " + std::to_string(shaderId) + " created.";
		Logger::print(Logger::Info, str);
		this->type = type;
		const GLchar* buf = source.c_str();
		glShaderSource(shaderId, 1, &buf, NULL);
	}
#else
	Shader::Shader(std::ifstream *file, Type type) {
		if (!file->is_open()) {
			std::string str = "Failed to open a file! Shader type: " + type;
			Logger::print(Logger::Error, str);
			return;
		}
		std::string source;
		std::string line;
		while (std::getline(*file, line)) {
			source += line + "\r\n";
		}
		file->close();
		std::string name;
		switch (type) {
		case Vertex:
			shaderId = glCreateShader(GL_VERTEX_SHADER);
			name = "Vertex";
			break;
		case Fragment:
			shaderId = glCreateShader(GL_FRAGMENT_SHADER);
			name = "Fragment";
			break;
		}
		this->type = type;
		std::string str = "Shader with type: " + name + ", id: " + std::to_string(shaderId) + " created.";
		Logger::print(Logger::Info, str);
		const GLchar* buf = source.c_str();
		glShaderSource(shaderId, 1, &buf, NULL);
	}
#endif
Shader::~Shader() {
	deleteShader();
}
bool Shader::compile() {
	glCompileShader(shaderId);
	GLint status;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
	if (status == GL_TRUE) {
		std::string msg = "Shader with id: " + std::to_string(shaderId) + " sucessfully compiled";
		Logger::print(Logger::Info, msg);
		return true;
	}
	else {
		GLchar infoLog[512];
		glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
		std::string str = "Failed to compile shader with id: " + std::to_string(shaderId) + " Error log:";
		Logger::print(Logger::Error, str);
		Logger::print(Logger::Error, "-------------------------");
		Logger::print(Logger::Error, infoLog);
		return false;
	}
}
GLuint Shader::getId() {
	return shaderId;
}
Shader::Type Shader::getType() {
	return type;
}
void Shader::deleteShader() {
	glDeleteShader(shaderId);
}