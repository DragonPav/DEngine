#pragma once
#include "Object3D.h"
#include "ObjectBuffer.h"
#include <vector>
class StaticModel {
private:
	Matrix4 model;
	std::vector<GLfloat> vertexBuffer;
	std::vector<GLuint> indexBuffer;
	std::vector<GLfloat> texBuffer;
	std::vector<GLfloat> normalBuffer;
	Texture* texture;
	GLuint vertBufSize = 0, indexBufSize = 0, texBufSize = 0, normalBufSize = 0;
	GLint shadowModelLocation = -1;
	ObjectBuffer b;
	Camera* cam = nullptr;
	RenderUtils* renderUtils = nullptr;
public:
	void setTexture(Texture* tex);
	void setCamera(Camera* camera);
	Texture* getTexture();
	Camera* getCamera();
	Matrix4& getModelMatrix();
    void begin();
	void render(Program* currentProgram, bool isShadowPass);
	void dispose();
	StaticModel() = delete;
	StaticModel(Camera* camera, RenderUtils* rend, std::vector<Object3D>& objs);
	~StaticModel();
};