#pragma once
#include <iostream>
#include <fstream>
#include <string>
#ifdef _WIN32
	#include <GL/glew.h>
#else
	#include <GLES3/gl32.h>
#endif
class Shader {
public:
	enum Type {
		Vertex,
		Fragment
	};
	Shader(std::ifstream *file, Type type);
	Shader() = delete;
	~Shader();
	bool compile();
	void deleteShader();
	GLuint getId();
	Type getType();
private:
	GLuint shaderId = 0;
	Type type = Vertex;
};