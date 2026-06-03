#pragma once
#include "Object3D.h"
#include "Camera.h"
#include "Vector2.h"
class ObjectCreator {
public:
	static Object3D createCustom(std::vector<GLfloat> verts, std::vector<GLuint> indxs, std::vector<GLfloat> texCoords, std::vector<GLfloat> normals, Camera* camera, Texture* texture);
	static Object3D createBox(Vector3 pos, Vector3 size, Camera* camera, Texture* texture);
	static Object3D createQuad(Vector3 pos, Vector2 size, Camera* camera, Texture* tex);
	static Object3D createSurface(Vector3 pos, Vector2 size, Camera* camera, Texture* tex);
	static Object3D createSphere(Vector3 pos, float radius, int sectorCount, int stackCount, Camera* camera, Texture* tex);
	static Object3D createBoxWireframe(Vector3 pos, Vector3 size, Camera* camera, Texture* texture);
	static Object3D createTerrain(Vector3 pos, int width, int height, float spacing, float scale, float heightScale, Camera* camera, Texture* tex);
	ObjectCreator() = default;
};