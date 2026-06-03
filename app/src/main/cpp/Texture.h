#pragma once
#include "Program.h"
#include "Vector3.h"
#include <vector>
class Texture {
public:
	class Config {
	public:
		bool useMipmap = true;
		int textureWrapS = GL_CLAMP_TO_EDGE;
		int textureWrapT = GL_CLAMP_TO_EDGE;
		int textureMinFilter = GL_NEAREST_MIPMAP_LINEAR;
		int textureMagFilter = GL_NEAREST;
		Config() = default;
	};
	GLint texSampleNum = 0;
	GLuint textureId = 0;
	std::vector<GLubyte> colorBuffer;
	int glType = GL_TEXTURE0;
	void dispose();
	Texture(std::vector<GLubyte>, Program* p, Config cfg);
	Texture(Program* p, AAsset* file, Config cfg);
	Texture() = delete;
};