#pragma once
#include "Lighting.h"
#include "ObjectBuffer.h"
class Lighting;
class RenderUtils {
public:
	int width, height;
	Lighting* lighting = nullptr;
	Program* p = nullptr;
	ObjectBuffer createStaticVAO(GLfloat* vertices, GLuint* indices, GLfloat* tex, GLfloat* normal, GLuint vertexBufSize, GLuint indexBufSize, GLuint texBufSize, GLuint normalBufferSize);
	RenderUtils() = delete;
	RenderUtils(Program* program, Lighting* li, int width, int height);
};