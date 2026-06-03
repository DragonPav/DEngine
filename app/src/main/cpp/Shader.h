#pragma once
#include <iostream>
#include <android/asset_manager.h>
#include <android/log.h>
#include <string>
#include <GLES3/gl32.h>
class Shader {
public:
    GLuint shaderId = 0;
    enum Type {
        Vertex,
        Fragment
    };
    Type type;
    Shader() = default;
    Shader(AAsset* asset, Type type);
    void compile();
    void deleteShader();
};