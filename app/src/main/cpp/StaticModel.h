#pragma once
#include "Object3D.h"
#include "ObjectBuffer.h"
#include <vector>
class StaticModel {
public:
    Matrix4 model;
    std::vector<GLfloat> vertexBuffer;
	std::vector<GLuint> indexBuffer;
	std::vector<GLfloat> texBuffer;
	std::vector<GLfloat> normalBuffer;
    Texture* texture;
    GLuint vertBufSize = 0, indexBufSize = 0, texBufSize = 0, normalBufSize = 0;
	ObjectBuffer b;
	Camera* cam = nullptr;
	RenderUtils* renderUtils = nullptr;
    void begin();
	void render(Program* currentProgram, bool isShadowPass);
	void end();
	void dispose();
	StaticModel() = delete;
	StaticModel(Camera* camera, RenderUtils* rend, std::vector<Object3D> objs);
};