#include "Texture.h"
#include <android/imagedecoder.h>
Texture::Texture(std::vector<GLubyte> rgb, Program* p, Config cfg) {
	texSampleNum = glGetUniformLocation(p->programId, "textureUnit");
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
Texture::Texture(Program* p, AAsset* file, Config cfg) {
	texSampleNum = glGetUniformLocation(p->programId, "textureUnit");
    AImageDecoder* decoder = nullptr;
	int result = AImageDecoder_createFromAAsset(file, &decoder);
    if (result != ANDROID_IMAGE_DECODER_SUCCESS) {
        __android_log_write(ANDROID_LOG_ERROR, "DEngine", "Failed to decode image");
        return;
    }
    const AImageDecoderHeaderInfo* header = AImageDecoder_getHeaderInfo(decoder);
    int width = AImageDecoderHeaderInfo_getWidth(header);
    int height = AImageDecoderHeaderInfo_getHeight(header);
    int format = AImageDecoderHeaderInfo_getAndroidBitmapFormat(header);
    if (format != ANDROID_BITMAP_FORMAT_RGB_565) {
        __android_log_write(ANDROID_LOG_ERROR, "DEngine", "No such format");
        AImageDecoder_delete(decoder);
        return;
    }
	size_t size = width * height * 3;
    int stride = AImageDecoder_getMinimumStride(decoder);
    int decodeResult = AImageDecoder_decodeImage(decoder, colorBuffer.data(), stride, size);
	if (decodeResult != ANDROID_IMAGE_DECODER_SUCCESS) {
        __android_log_write(ANDROID_LOG_ERROR, "DEngine", "Failed to decode texture");
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
    AImageDecoder_delete(decoder);
}
void Texture::dispose() {
	glDeleteTextures(1, &textureId);
}