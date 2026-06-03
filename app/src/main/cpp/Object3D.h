#pragma once
#include <vector>
#include "Texture.h"
#include "Camera.h"
#include "ObjectBuffer.h"
#include "BoundingBox.h"
class Camera;
class RenderUtils;
class Object3D {
public:
	Vector3 position;
	Matrix4 model;
	BoundingBox bounds;
	std::vector<GLfloat> vertexBuffer;
	std::vector<GLuint> indexBuffer;
	std::vector<GLfloat> texBuffer;
	std::vector<GLfloat> normalBuffer;
	Texture* texture;
	ObjectBuffer b;
	Camera* cam = nullptr;
	RenderUtils* renderUtils = nullptr;
	GLenum polygonMode = GL_TRIANGLES;
	void setup();
	void begin();
	void render(Program* currentProgram, bool isShadowPass);
	void end();
	void dispose();
	Object3D() = delete;
	Object3D(Camera* camera, RenderUtils* rend, Texture* tex);
};