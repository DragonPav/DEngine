#pragma once
#include <iostream>
#include <fstream>
#include <string>
#ifdef _WIN32
	#include <GL/glew.h>
#else
	#include <sstream>
	#include <GLES3/gl32.h>
	#include <android/asset_manager.h>
#endif
class Shader {
public:
	enum Type {
		Vertex,
		Fragment
	};
	#ifdef _WIN32
		Shader(std::ifstream *file, Type type);
	#else
		Shader(AAsset *file, Type type);
	#endif
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