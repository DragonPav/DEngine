#pragma once
#include <iostream>
#include <fstream>
#include <string>
#ifdef __ANDROID__
	#include <sstream>
	#include <GLES3/gl32.h>
	#include <android/asset_manager.h>
#else
	#include <GL/glew.h>
#endif
class Shader {
public:
	enum Type {
		Vertex,
		Fragment
	};
	#ifdef __ANDROID__
		Shader(AAsset *file, Type type);
	#else
		Shader(std::ifstream *file, Type type);
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