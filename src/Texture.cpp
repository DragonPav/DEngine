#include "Texture.h"
#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"
#ifdef __ANDROID__
	#include <android/asset_manager.h>
#endif
Texture::Texture(std::vector<GLubyte> rgba, Program* p, Config cfg) {
	texSampleNum = glGetUniformLocation(p->getId(), "textureUnit");
	int i = 0;
	while (i < 256 * 256 * 4) {
		colorBuffer.push_back(rgba[0]);
		i++;
		colorBuffer.push_back(rgba[1]);
		i++;
		colorBuffer.push_back(rgba[2]);
		i++;
		colorBuffer.push_back(rgba[3]);
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, colorBuffer.data());
	if (cfg.useMipmap) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}
#ifdef _WIN32
	Texture::Texture(Program* p, std::string file, Config cfg) {
		texSampleNum = glGetUniformLocation(p->getId(), "textureUnit");
		int width, height, bpp;
		unsigned char* dat = stbi_load(file.c_str(), &width, &height, &bpp, 4);
		size_t size = static_cast<size_t>(width) * height * 4;
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, colorBuffer.data());
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
		unsigned char* dat = stbi_load_from_memory((const stbi_uc*) buffer, size, &width, &height, &bpp, 4);
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, colorBuffer.data());
		if (glGetError() != GL_NO_ERROR) {
			std::cerr << "Texture setup failure: GL error: " << glGetError() << std::endl;
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
std::vector<GLfloat> Texture::toNormalBuffer() {
	std::vector<GLfloat> result;
	result.reserve(width * height * 4);
	for (int i = 0; i < width * height * 4; i += 4) {
		float r = colorBuffer[i] / 255.0f;
		float g = colorBuffer[i + 1] / 255.0f;
		float b = colorBuffer[i + 2] / 255.0f;

		float nx = r * 2.0f - 1.0f;
        float ny = g * 2.0f - 1.0f;
        float nz = b * 2.0f - 1.0f;

		result.push_back(nx);
        result.push_back(ny);
        result.push_back(nz);
	}
	return result;
}
void Texture::dispose() {
	glDeleteTextures(1, &textureId);
	textureId = 0;
	colorBuffer.clear();
}