#include "Shader.h"
Shader::Shader(AAsset *asset, Type type) {
    this->type = type;
    switch (type) {
        case Vertex:
            shaderId = glCreateShader(GL_VERTEX_SHADER);
            __android_log_write(ANDROID_LOG_INFO, "DEngine", ("Created shader with id: " + std::to_string(shaderId) + ", type: Vertex").c_str());
            break;
        case Fragment:
            shaderId = glCreateShader(GL_FRAGMENT_SHADER);
            __android_log_write(ANDROID_LOG_INFO, "DEngine", ("Created shader with id: " + std::to_string(shaderId) + ", type: Fragment").c_str());
            break;
    }
    size_t fileLength = AAsset_getLength(asset);
    char* content = (char*) malloc(fileLength + 1);
    AAsset_read(asset, content, fileLength);
    content[fileLength] = '\0';
    AAsset_close(asset);
    glShaderSource(shaderId, 1, &content, nullptr);
}
void Shader::compile() {
    glCompileShader(shaderId);
    GLint status;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
    if (status) {
        __android_log_write(ANDROID_LOG_INFO, "DEngine", ("Shader with id: " + std::to_string(shaderId) + " successfully compiled").c_str());
    } else {
        GLchar infoLog[512];
        glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
        __android_log_write(ANDROID_LOG_ERROR, "DEngine", ("Shader with id: " + std::to_string(shaderId) + " not compiled:").c_str());
        __android_log_write(ANDROID_LOG_WARN, "DEngine", infoLog);
    }
}
void Shader::deleteShader() {
    glDeleteShader(shaderId);
}