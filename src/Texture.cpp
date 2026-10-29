#include "Texture.h"
#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"
#ifdef __ANDROID__
	#include <android/asset_manager.h>
#endif
#include "Logger.h"
Texture::Texture(std::vector<GLubyte> rgba, Program* p, Config cfg) {
	texSampleNum = glGetUniformLocation(p->getId(), "textureUnit");
	size_t totalBytes = 256 * 256 * 4;
    colorBuffer.resize(totalBytes);
	for (size_t i = 0; i < totalBytes; i += 4) {
        colorBuffer[i] = rgba[0];
        colorBuffer[i + 1] = rgba[1];
        colorBuffer[i + 2] = rgba[2];
        colorBuffer[i + 3] = rgba[3];
    }
	GLuint tmpBuf[1];
	glGenTextures(1, tmpBuf);
	textureId = tmpBuf[0];
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, cfg.textureWrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, cfg.textureWrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, cfg.textureMinFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, cfg.textureMagFilter);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, colorBuffer.data());
	if (cfg.useMipmap) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}
#ifdef __ANDROID__
	Texture::Texture(Program* p, AAsset* fileBuffer, Config cfg) {
		texSampleNum = glGetUniformLocation(p->getId(), "textureUnit");
		size_t size = AAsset_getLength(fileBuffer);
		const void* buffer = AAsset_getBuffer(fileBuffer);
		int bpp;
		unsigned char* dat = stbi_load_from_memory((const stbi_uc*) buffer, size, &width, &height, &bpp, 4);
		if (dat == nullptr) {
			Logger::print(Logger::Error, "[Texture]: Texture setup failure: STBI error: " + std::string(stbi_failure_reason()));
			return;
		}
		GLuint tmpBuf[1];
		glGenTextures(1, tmpBuf);
		textureId = tmpBuf[0];
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, cfg.textureWrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, cfg.textureWrapT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, cfg.textureMinFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, cfg.textureMagFilter);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dat);
		GLenum err = glGetError();
		if (err != GL_NO_ERROR) {
			Logger::print(Logger::Error, "[Texture]: Texture setup failure: GL error: " + std::to_string(err));
			return;
		}
		if (cfg.useMipmap) {
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		AAsset_close(fileBuffer);
		stbi_image_free(dat);
	}
#else
	Texture::Texture(Program* p, std::string file, Config cfg) {
		texSampleNum = glGetUniformLocation(p->getId(), "textureUnit");
		int w, h, bpp;
		unsigned char* dat = stbi_load(file.c_str(), &w, &h, &bpp, 4);
		this->width = w;  
    	this->height = h;
		size_t size = static_cast<size_t>(width) * height * 4;
		colorBuffer.insert(colorBuffer.begin(), dat, dat + size);
		if (dat == nullptr) {
			Logger::print(Logger::Error, "[Texture]: Texture setup failure: STBI error: " + std::string(stbi_failure_reason()));
			return;
		}
		GLuint tmpBuf[1];
		glGenTextures(1, tmpBuf);
		textureId = tmpBuf[0];
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, cfg.textureWrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, cfg.textureWrapT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, cfg.textureMinFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, cfg.textureMagFilter);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, colorBuffer.data());
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		GLenum err = glGetError();
		if (err != GL_NO_ERROR) {
			Logger::print(Logger::Error, "[Texture]: Texture setup failure: GL error: " + std::to_string(err));
		}
		if (cfg.useMipmap) {
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(dat);
	}
#endif
Texture::Texture(GLubyte* rgba, Program* p, Config cfg, GLuint width, GLuint height) {
	texSampleNum = glGetUniformLocation(p->getId(), "textureUnit");
	GLuint tmpBuf[1];
	glGenTextures(1, tmpBuf);
	textureId = tmpBuf[0];
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, cfg.textureWrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, cfg.textureWrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, cfg.textureMinFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, cfg.textureMagFilter);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
	if (cfg.useMipmap) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}
GLint Texture::getTextureSample() {
	return texSampleNum;
}
GLuint Texture::getTextureId() {
	return textureId;
}
GLint Texture::getGLType() {
	return glType;
}
void Texture::dispose() {
	glDeleteTextures(1, &textureId);
	textureId = 0;
	colorBuffer.clear();
}