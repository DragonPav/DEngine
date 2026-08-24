#include "Texture.h"
#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"
#ifdef __ANDROID__
	#include <android/asset_manager.h>
#endif
Texture::Texture(std::vector<GLubyte> rgb, Program* p, Config cfg) {
	texSampleNum = glGetUniformLocation(p->getId(), "textureUnit");
	int i = 0;
	while (i < 256 * 256 * 3) {
		colorBuffer.push_back(rgb[0]);
		i++;
		colorBuffer.push_back(rgb[1]);
		i++;
		colorBuffer.push_back(rgb[2]);
		i++;
	}
	GLuint tmpBuf[1];
	glGenTextures(1, tmpBuf);
	textureId = tmpBuf[0];
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, cfg.textureWrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, cfg.textureWrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, cfg.textureMinFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, cfg.textureMagFilter);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, colorBuffer.data());
	if (cfg.useMipmap) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}
#ifdef _WIN32
	Texture::Texture(Program* p, std::string file, Config cfg) {
		texSampleNum = glGetUniformLocation(p->getId(), "textureUnit");
		int width, height, bpp;
		unsigned char* dat = stbi_load(file.c_str(), &width, &height, &bpp, 0);
		size_t size = static_cast<size_t>(width) * height * bpp;
		colorBuffer.insert(colorBuffer.begin(), dat, dat + size);
		if (stbi_failure_reason()) {
			std::cerr << "STBI error: " << stbi_failure_reason() << std::endl;
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, colorBuffer.data());
		if (glGetError() != GL_NO_ERROR) {
			std::cerr << "Texture setup failure: gl error: " << glGetError() << std::endl;
		}
		if (cfg.useMipmap) {
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(dat);
	}
#else
	Texture::Texture(Program* p, AAsset* fileBuffer, Config cfg) {
		texSampleNum = glGetUniformLocation(p->getId(), "textureUnit");
		size_t size = AAsset_getLength(fileBuffer);
		const void* buffer = AAsset_getBuffer(fileBuffer);
		int width, height, bpp;
		unsigned char* dat = stbi_load_from_memory((const stbi_uc*) buffer, size, &width, &height, &bpp, 0);
		colorBuffer.insert(colorBuffer.begin(), dat, dat + size);
		if (stbi_failure_reason()) {
			std::cerr << "STBI error: " << stbi_failure_reason() << std::endl;
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, colorBuffer.data());
		if (glGetError() != GL_NO_ERROR) {
			std::cerr << "Texture setup failure: gl error: " << glGetError() << std::endl;
		}
		if (cfg.useMipmap) {
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(dat);
	}
#endif
Texture::Texture(GLubyte* rgb, Program* p, Config cfg, GLuint width, GLuint height) {
	texSampleNum = glGetUniformLocation(p->getId(), "textureUnit");
	GLuint tmpBuf[1];
	glGenTextures(1, tmpBuf);
	textureId = tmpBuf[0];
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, cfg.textureWrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, cfg.textureWrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, cfg.textureMinFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, cfg.textureMagFilter);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb);
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
}