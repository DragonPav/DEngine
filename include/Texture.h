#pragma once
#include "Program.h"
#include "Vector3.h"
#include <vector>
#ifdef __ANDROID__
	#include <android/asset_manager.h>
#endif
class Texture {
private:
	GLint texSampleNum = 0;
	GLuint textureId = 0;
	std::vector<GLubyte> colorBuffer;
	GLint glType = GL_TEXTURE0;
	int width = 0;
	int height = 0;
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
	GLint getTextureSample();
	GLuint getTextureId();
	GLint getGLType();
	std::vector<GLfloat> toNormalBuffer();
	void dispose();
	Texture(std::vector<GLubyte> rgba, Program* p, Config cfg);
	#ifdef _WIN32
		Texture(Program* p, std::string file, Config cfg);
	#else
		Texture(Program* p, AAsset* fileBuffer, Config cfg);
	#endif
	Texture(GLubyte* rgba, Program* p, Config cfg, GLuint width, GLuint height);
	Texture() = delete;
};