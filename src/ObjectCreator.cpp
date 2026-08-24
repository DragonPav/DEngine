#include "ObjectCreator.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
std::unique_ptr<Object3D> ObjectCreator::createCustom(std::vector<GLfloat> verts, std::vector<GLuint> indxs, std::vector<GLfloat> texCoords, std::vector<GLfloat> normals, Camera* camera, Texture* texture) {
	if (camera == nullptr) {
		throw std::runtime_error("Camera is null");
	}
	if (texture == nullptr) {
		throw std::runtime_error("Texture is null");
	}
	std::unique_ptr<Object3D> ret = std::make_unique<Object3D>(camera, camera->getRenderUtils(), texture);
	ret->vertexBuffer = verts;
	ret->indexBuffer = indxs;
	ret->texBuffer = texCoords;
	ret->normalBuffer = normals;
	ret->setup();
	return ret;
}
std::unique_ptr<Object3D> ObjectCreator::createBox(Vector3 pos, Vector3 size, Camera* camera, Texture* texture) {
	if (camera == nullptr) {
		throw std::runtime_error("Camera is null");
	}
	if (texture == nullptr) {
		throw std::runtime_error("Texture is null");
	}
	std::unique_ptr<Object3D> ret = std::make_unique<Object3D>(camera, camera->getRenderUtils(), texture);
	GLfloat halfWidth = size.getX() / 2;
	GLfloat halfHeight = size.getY() / 2;
	GLfloat halfDepth = size.getZ() / 2;
	GLfloat vertices[] = {
		//back
		pos.getX() - halfWidth, pos.getY() - halfHeight, pos.getZ() + halfDepth,
		pos.getX() + halfWidth, pos.getY() - halfHeight, pos.getZ() + halfDepth,
		pos.getX() - halfWidth, pos.getY() + halfHeight, pos.getZ() + halfDepth,
		pos.getX() + halfWidth, pos.getY() + halfHeight, pos.getZ() + halfDepth,
		//left
		pos.getX() - halfWidth, pos.getY() - halfHeight, pos.getZ() - halfDepth,
		pos.getX() - halfWidth, pos.getY() - halfHeight, pos.getZ() + halfDepth,
		pos.getX() - halfWidth, pos.getY() + halfHeight, pos.getZ() - halfDepth,
		pos.getX() - halfWidth, pos.getY() + halfHeight, pos.getZ() + halfDepth,
		//front
		pos.getX() - halfWidth, pos.getY() - halfHeight, pos.getZ() - halfDepth,
		pos.getX() + halfWidth, pos.getY() - halfHeight, pos.getZ() - halfDepth,
		pos.getX() - halfWidth, pos.getY() + halfHeight, pos.getZ() - halfDepth,
		pos.getX() + halfWidth, pos.getY() + halfHeight, pos.getZ() - halfDepth,
		//right
		pos.getX() + halfWidth, pos.getY() - halfHeight, pos.getZ() + halfDepth,
		pos.getX() + halfWidth, pos.getY() - halfHeight, pos.getZ() - halfDepth,
		pos.getX() + halfWidth, pos.getY() + halfHeight, pos.getZ() + halfDepth,
		pos.getX() + halfWidth, pos.getY() + halfHeight, pos.getZ() - halfDepth,
		//top
		pos.getX() - halfWidth, pos.getY() + halfHeight, pos.getZ() + halfDepth,
		pos.getX() + halfWidth, pos.getY() + halfHeight, pos.getZ() + halfDepth,
		pos.getX() - halfWidth, pos.getY() + halfHeight, pos.getZ() - halfDepth,
		pos.getX() + halfWidth, pos.getY() + halfHeight, pos.getZ() - halfDepth,
		//bottom
		pos.getX() - halfWidth, pos.getY() - halfHeight, pos.getZ() + halfDepth,
		pos.getX() + halfWidth, pos.getY() - halfHeight, pos.getZ() + halfDepth,
		pos.getX() - halfWidth, pos.getY() - halfHeight, pos.getZ() - halfDepth,
		pos.getX() + halfWidth, pos.getY() - halfHeight, pos.getZ() - halfDepth
	};
	GLuint indices[] = {
		//back
		0, 1, 2,
		2, 3, 1,
		//left
		4, 5, 6,
		6, 7, 5,
		//front
		8, 9, 10,
		10, 11, 9,
		//right
		12, 13, 14,
		14, 15, 13,
		//top
		16, 17, 18,
		18, 19, 17,
		//bottom
		20, 21, 22,
		22, 23, 21
	};
	GLfloat texCoords[] = {
		//back
		0, 1,
		1, 1,
		0, 0,
		1, 0,
		//left
		0, 1,
		1, 1,
		0, 0,
		1, 0,
		//front
		1, 1,
		0, 1,
		1, 0,
		0, 0,
		//right
		0, 1,
		1, 1,
		0, 0,
		1, 0,
		//top
		0, 1,
		1, 1,
		0, 0,
		1, 0,
		//bottom
		1, 1,
		0, 1,
		1, 0,
		0, 0
	};
	GLfloat normals[] = {
		//back
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		//left
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		//front
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		//right
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		//top
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		//bottom
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,
		0, -1, 0
	};
	ret->position = pos;
	Vector3 min(pos.getX() - halfWidth, pos.getY() - halfHeight, pos.getZ() - halfDepth);
	Vector3 max(pos.getX() + halfWidth, pos.getY() + halfHeight, pos.getZ() + halfDepth);
	ret->bounds = BoundingBox(min, max);
	for (int i = 0; i < sizeof(vertices) / sizeof(GLfloat); i++) {
		ret->vertexBuffer.push_back(vertices[i]);
	}
	for (int i = 0; i < sizeof(indices) / sizeof(GLuint); i++) {
		ret->indexBuffer.push_back(indices[i]);
	}
	for (int i = 0; i < sizeof(texCoords) / sizeof(GLfloat); i++) {
		ret->texBuffer.push_back(texCoords[i]);
	}
	if (camera->getRenderUtils()->getLighting() != nullptr) {
		for (int i = 0; i < sizeof(normals) / sizeof(GLfloat); i++) {
			ret->normalBuffer.push_back(normals[i]);
		}
	}
	ret->setup();
	return ret;
}
std::unique_ptr<Object3D> ObjectCreator::createQuad(Vector3 pos, Vector2 size, Camera* camera, Texture* tex) {
	if (camera == nullptr) {
		throw std::runtime_error("Camera is null");
	}
	if (tex == nullptr) {
		throw std::runtime_error("Texture is null");
	}
	std::unique_ptr<Object3D> ret = std::make_unique<Object3D>(camera, camera->getRenderUtils(), tex);
	float halfWidth = size.getX() / 2;
	float halfHeight = size.getY() / 2;
	GLfloat vertices[] = {
		pos.getX() - halfWidth, pos.getY() - halfHeight, pos.getZ(),
		pos.getX() + halfWidth, pos.getY() - halfHeight, pos.getZ(),
		pos.getX() - halfWidth, pos.getY() + halfHeight, pos.getZ(),
		pos.getX() + halfWidth, pos.getY() + halfHeight, pos.getZ()
	};
	GLuint indices[] = {
		0, 1, 2,
		2, 3, 1
	};
	GLfloat texCoords[] = {
		0, 1,
		1, 1,
		0, 0,
		1, 0
	};
	GLfloat normals[] = {
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1
	};
	ret->position = pos;
	Vector3 min(pos.getX() - halfWidth, pos.getY() - halfHeight, pos.getZ());
	Vector3 max(pos.getX() + halfWidth, pos.getY() + halfHeight, pos.getZ());
	ret->bounds = BoundingBox(min, max);
	for (size_t i = 0; i < sizeof(vertices) / sizeof(GLfloat); i++) {
		ret->vertexBuffer.push_back(vertices[i]);
	}
	for (size_t i = 0; i < sizeof(indices) / sizeof(GLuint); i++) {
		ret->indexBuffer.push_back(indices[i]);
	}
	for (size_t i = 0; i < sizeof(texCoords) / sizeof(GLfloat); i++) {
		ret->texBuffer.push_back(texCoords[i]);
	}
	if (camera->getRenderUtils()->getLighting() != nullptr) {
		for (size_t i = 0; i < sizeof(normals) / sizeof(GLfloat); i++) {
			ret->normalBuffer.push_back(normals[i]);
		}
	}
	ret->setup();
	return ret;
}
std::unique_ptr<Object3D> ObjectCreator::createSurface(Vector3 pos, Vector2 size, Camera* camera, Texture* tex) {
	if (camera == nullptr) {
		throw std::runtime_error("Camera is null");
	}
	if (tex == nullptr) {
		throw std::runtime_error("Texture is null");
	}
	std::unique_ptr<Object3D> ret = std::make_unique<Object3D>(camera, camera->getRenderUtils(), tex);
	GLfloat halfWidth = size.getX() / 2;
	GLfloat halfDepth = size.getY() / 2;
	GLfloat vertices[] = {
		pos.getX() - halfWidth, pos.getY(), pos.getZ() + halfDepth,
		pos.getX() + halfWidth, pos.getY(), pos.getZ() + halfDepth,
		pos.getX() - halfWidth, pos.getY(), pos.getZ() - halfDepth,
		pos.getX() + halfWidth, pos.getY(), pos.getZ() - halfDepth
	};
	GLuint indices[] = {
		0, 1, 2,
		2, 3, 1
	};
	GLfloat texCoords[] = {
		0, 1,
		1, 1,
		0, 0,
		1, 0
	};
	GLfloat normals[] = {
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0
	};
	ret->position = pos;
	Vector3 min(pos.getX() - halfWidth, pos.getY(), pos.getZ() - halfDepth);
	Vector3 max(pos.getX() + halfWidth, pos.getY(), pos.getZ() + halfDepth);
	ret->bounds = BoundingBox(min, max);
	for (size_t i = 0; i < sizeof(vertices) / sizeof(GLfloat); i++) {
		ret->vertexBuffer.push_back(vertices[i]);
	}
	for (size_t i = 0; i < sizeof(indices) / sizeof(GLuint); i++) {
		ret->indexBuffer.push_back(indices[i]);
	}
	for (size_t i = 0; i < sizeof(texCoords) / sizeof(GLfloat); i++) {
		ret->texBuffer.push_back(texCoords[i]);
	}
	if (camera->getRenderUtils()->getLighting() != nullptr) {
		for (size_t i = 0; i < sizeof(normals) / sizeof(GLfloat); i++) {
			ret->normalBuffer.push_back(normals[i]);
		}
	}
	ret->setup();
	return ret;
}
std::unique_ptr<Object3D> ObjectCreator::createSphere(Vector3 pos, float radius, int sectorCount, int stackCount, Camera* camera, Texture* tex) {
	if (camera == nullptr) {
		throw std::runtime_error("Camera is null");
	}
	if (tex == nullptr) {
		throw std::runtime_error("Texture is null");
	}
	std::unique_ptr<Object3D> ret = std::make_unique<Object3D>(camera, camera->getRenderUtils(), tex);
	float x = 0, y = 0, z = 0, xy;
	float nx, ny, nz, lengthInv = 1.0f / radius;
	float s, t;

	float sectorStep = 2 * 3.14159f / sectorCount;
	float stackStep = 3.14159f / stackCount;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stackCount; i++) {
		stackAngle = (float)3.14159 / 2 - i * stackStep;
		xy = radius * cosf(stackAngle);
		z = radius * sinf(stackAngle);
		for (int j = 0; j <= sectorCount; j++) {
			sectorAngle = j * sectorStep;
			x = xy * cosf(sectorAngle);
			y = xy * sinf(sectorAngle);
			ret->vertexBuffer.push_back(x + pos.getX());
			ret->vertexBuffer.push_back(y + pos.getY());
			ret->vertexBuffer.push_back(z + pos.getZ());
			if (ret->renderUtils->getLighting() != nullptr) {
				nx = x * lengthInv;
				ny = y * lengthInv;
				nz = z * lengthInv;
				ret->normalBuffer.push_back(nx);
				ret->normalBuffer.push_back(ny);
				ret->normalBuffer.push_back(nz);
			}
			s = (float)j / sectorCount;
			t = (float)i / stackCount;
			ret->texBuffer.push_back(s);
			ret->texBuffer.push_back(t);
		}
	}
	GLuint k1, k2;
	for (int i = 0; i < stackCount; i++) {
		k1 = i * (sectorCount + 1);
		k2 = k1 + sectorCount + 1;
		for (int j = 0; j < sectorCount; j++, k1++, k2++) {
			if (i != 0) {
				ret->indexBuffer.push_back(k1);
				ret->indexBuffer.push_back(k2);
				ret->indexBuffer.push_back(k1 + 1);
			}
			if (i != (stackCount - 1)) {
				ret->indexBuffer.push_back(k1 + 1);
				ret->indexBuffer.push_back(k2);
				ret->indexBuffer.push_back(k2 + 1);
			}
		}
	}
	ret->bounds = BoundingBox(
		Vector3(pos.getX() - radius, pos.getY() - radius, pos.getZ() - radius),
		Vector3(pos.getX() + radius, pos.getY() + radius, pos.getZ() + radius)
	);
	ret->position = pos;
	ret->setup();
	return ret;
}
std::unique_ptr<Object3D> ObjectCreator::createBoxWireframe(Vector3 pos, Vector3 size, Camera* camera, Texture* texture) {
	if (camera == nullptr) {
		throw std::runtime_error("Camera is null");
	}
	if (texture == nullptr) {
		throw std::runtime_error("Texture is null");
	}
	std::unique_ptr<Object3D> ret = std::make_unique<Object3D>(camera, camera->getRenderUtils(), texture);
	GLfloat halfWidth = size.getX() / 2;
	GLfloat halfHeight = size.getY() / 2;
	GLfloat halfDepth = size.getZ() / 2;
	GLfloat vertices[] = {
		//back
		pos.getX() - halfWidth, pos.getY() - halfHeight, pos.getZ() + halfDepth,
		pos.getX() + halfWidth, pos.getY() - halfHeight, pos.getZ() + halfDepth,
		pos.getX() - halfWidth, pos.getY() + halfHeight, pos.getZ() + halfDepth,
		pos.getX() + halfWidth, pos.getY() + halfHeight, pos.getZ() + halfDepth,
		//left
		pos.getX() - halfWidth, pos.getY() - halfHeight, pos.getZ() - halfDepth,
		pos.getX() - halfWidth, pos.getY() - halfHeight, pos.getZ() + halfDepth,
		pos.getX() - halfWidth, pos.getY() + halfHeight, pos.getZ() - halfDepth,
		pos.getX() - halfWidth, pos.getY() + halfHeight, pos.getZ() + halfDepth,
		//front
		pos.getX() - halfWidth, pos.getY() - halfHeight, pos.getZ() - halfDepth,
		pos.getX() + halfWidth, pos.getY() - halfHeight, pos.getZ() - halfDepth,
		pos.getX() - halfWidth, pos.getY() + halfHeight, pos.getZ() - halfDepth,
		pos.getX() + halfWidth, pos.getY() + halfHeight, pos.getZ() - halfDepth,
		//right
		pos.getX() + halfWidth, pos.getY() - halfHeight, pos.getZ() + halfDepth,
		pos.getX() + halfWidth, pos.getY() - halfHeight, pos.getZ() - halfDepth,
		pos.getX() + halfWidth, pos.getY() + halfHeight, pos.getZ() + halfDepth,
		pos.getX() + halfWidth, pos.getY() + halfHeight, pos.getZ() - halfDepth,
		//top
		pos.getX() - halfWidth, pos.getY() + halfHeight, pos.getZ() + halfDepth,
		pos.getX() + halfWidth, pos.getY() + halfHeight, pos.getZ() + halfDepth,
		pos.getX() - halfWidth, pos.getY() + halfHeight, pos.getZ() - halfDepth,
		pos.getX() + halfWidth, pos.getY() + halfHeight, pos.getZ() - halfDepth,
		//bottom
		pos.getX() - halfWidth, pos.getY() - halfHeight, pos.getZ() + halfDepth,
		pos.getX() + halfWidth, pos.getY() - halfHeight, pos.getZ() + halfDepth,
		pos.getX() - halfWidth, pos.getY() - halfHeight, pos.getZ() - halfDepth,
		pos.getX() + halfWidth, pos.getY() - halfHeight, pos.getZ() - halfDepth
	};
	GLuint indices[] = {
		//back
		0, 1,
		1, 2,
		2, 0,
		2, 3,
		//left
		4, 5,
		5, 6,
		6, 4,
		6, 7,
		//front
		8, 9,
		9, 10,
		10, 8,
		10, 11,
		//right
		12, 13,
		13, 14,
		14, 12,
		14, 15,
		//top
		16, 17,
		17, 18,
		18, 16,
		18, 19,
		//bottom
		20, 21,
		21, 22,
		22, 20,
		22, 23
	};
	GLfloat texCoords[] = {
		//back
		0, 1,
		1, 1,
		0, 0,
		1, 0,
		//left
		0, 1,
		1, 1,
		0, 0,
		1, 0,
		//front
		1, 1,
		0, 1,
		1, 0,
		0, 0,
		//right
		0, 1,
		1, 1,
		0, 0,
		1, 0,
		//top
		0, 1,
		1, 1,
		0, 0,
		1, 0,
		//bottom
		1, 1,
		0, 1,
		1, 0,
		0, 0
	};
	GLfloat normals[] = {
		//back
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		//left
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		//front
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		//right
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		//top
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		//bottom
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,
		0, -1, 0
	};
	ret->position = pos;
	Vector3 min(pos.getX() - halfWidth, pos.getY() - halfHeight, pos.getZ() - halfDepth);
	Vector3 max(pos.getX() + halfWidth, pos.getY() + halfHeight, pos.getZ() + halfDepth);
	ret->bounds = BoundingBox(min, max);
	ret->polygonMode = GL_LINES;
	for (int i = 0; i < sizeof(vertices) / sizeof(GLfloat); i++) {
		ret->vertexBuffer.push_back(vertices[i]);
	}
	for (int i = 0; i < sizeof(indices) / sizeof(GLuint); i++) {
		ret->indexBuffer.push_back(indices[i]);
	}
	for (int i = 0; i < sizeof(texCoords) / sizeof(GLfloat); i++) {
		ret->texBuffer.push_back(texCoords[i]);
	}
	if (camera->getRenderUtils()->getLighting() != nullptr) {
		for (int i = 0; i < sizeof(normals) / sizeof(GLfloat); i++) {
			ret->normalBuffer.push_back(normals[i]);
		}
	}
	ret->setup();
	return ret;
}
std::unique_ptr<Object3D> ObjectCreator::createTerrain(Vector3 pos, int width, int height, float spacing, float scale, float heightScale, Camera* camera, Texture* tex) {
	std::unique_ptr<Object3D> ret = std::make_unique<Object3D>(camera, camera->getRenderUtils(), tex);
	ret->position = pos;
	std::vector<std::vector<float>> heightMap(height, std::vector<float>(width));
	float halfWidth = (width - 1) * spacing / 2.0f;
	float halfHeight = (height - 1) * spacing / 2.0f;
	float minX = pos.getX() - halfWidth;
	float maxX = pos.getX() + halfWidth;
	float minZ = pos.getZ() - halfHeight;
	float maxZ = pos.getZ() + halfHeight;
	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::lowest();
	for (int z = 0; z < height; ++z) {
		for (int x = 0; x < width; ++x) {
			float localX = x * spacing - halfWidth;
			float localZ = z * spacing - halfHeight;
			float noiseX = static_cast<float>(x) * scale;
			float noiseZ = static_cast<float>(z) * scale;
			float localY = glm::perlin(glm::vec2(noiseX, noiseZ)) * heightScale;
			heightMap[z][x] = localY;
			float absoluteY = pos.getY() + localY;
			if (absoluteY < minY) minY = absoluteY;
			if (absoluteY > maxY) maxY = absoluteY;
			ret->vertexBuffer.push_back(pos.getX() + localX);
			ret->vertexBuffer.push_back(absoluteY);
			ret->vertexBuffer.push_back(pos.getZ() + localZ);
			float u = static_cast<float>(x) / (width - 1);
			float v = static_cast<float>(z) / (height - 1);
			ret->texBuffer.push_back(u);
			ret->texBuffer.push_back(v);
		}
	}
	ret->bounds = BoundingBox(Vector3(minX, minY, minZ), Vector3(maxX, maxY, maxZ));
	for (int z = 0; z < height - 1; ++z) {
		for (int x = 0; x < width - 1; ++x) {
			GLuint topLeft = z * width + x;
			GLuint topRight = topLeft + 1;
			GLuint bottomLeft = (z + 1) * width + x;
			GLuint bottomRight = bottomLeft + 1;
			ret->indexBuffer.push_back(topLeft);
			ret->indexBuffer.push_back(bottomLeft);
			ret->indexBuffer.push_back(topRight);
			ret->indexBuffer.push_back(topRight);
			ret->indexBuffer.push_back(bottomLeft);
			ret->indexBuffer.push_back(bottomRight);
		}
	}
	if (camera->getRenderUtils()->getLighting() != nullptr) {
		std::vector<glm::vec3> tempNormals(width * height, glm::vec3(0.0f));
		for (size_t i = 0; i < ret->indexBuffer.size(); i += 3) {
			GLuint idx0 = ret->indexBuffer[i];
			GLuint idx1 = ret->indexBuffer[i + 1];
			GLuint idx2 = ret->indexBuffer[i + 2];
			glm::vec3 p0(ret->vertexBuffer[idx0 * 3], ret->vertexBuffer[idx0 * 3 + 1], ret->vertexBuffer[idx0 * 3 + 2]);
			glm::vec3 p1(ret->vertexBuffer[idx1 * 3], ret->vertexBuffer[idx1 * 3 + 1], ret->vertexBuffer[idx1 * 3 + 2]);
			glm::vec3 p2(ret->vertexBuffer[idx2 * 3], ret->vertexBuffer[idx2 * 3 + 1], ret->vertexBuffer[idx2 * 3 + 2]);
			glm::vec3 edge1 = p1 - p0;
			glm::vec3 edge2 = p2 - p0;
			glm::vec3 normal = glm::cross(edge1, edge2);
			tempNormals[idx0] += normal;
			tempNormals[idx1] += normal;
			tempNormals[idx2] += normal;
		}
		for (int i = 0; i < width * height; ++i) {
			glm::vec3 n = tempNormals[i];
			if (glm::length(n) > 0.0f) {
				n = glm::normalize(n);
			}
			else {
				n = glm::vec3(0.0f, 1.0f, 0.0f);
			}
			ret->normalBuffer.push_back(n.x);
			ret->normalBuffer.push_back(n.y);
			ret->normalBuffer.push_back(n.z);
		}
	}
	ret->setup();
	return ret;
}
std::unique_ptr<Object3D> ObjectCreator::createFromOBJ(std::ifstream* file, Vector3 pos, Camera* camera, Texture* tex) {
	if (camera == nullptr) throw std::runtime_error("Camera is null");
	if (tex == nullptr) throw std::runtime_error("Texture is null");
	if (!file->is_open()) throw std::runtime_error("Failed to open OBJ file");
	std::unique_ptr<Object3D> ret = std::make_unique<Object3D>(camera, camera->getRenderUtils(), tex);
	ret->position = pos;
	std::vector<Vector3> raw_positions;
	std::vector<Vector2> raw_texcoords;
	std::vector<Vector3> raw_normals;
	struct ObjIndices {
		int vIdx, vtIdx, vnIdx;
	};
	std::vector<ObjIndices> historyOfVertices;
	float minX = 1e9f, minY = 1e9f, minZ = 1e9f;
	float maxX = -1e9f, maxY = -1e9f, maxZ = -1e9f;
	bool hasVertices = false;
	std::string line;
	while (std::getline(*file, line)) {
		std::istringstream ss(line);
		std::string type;
		ss >> type;
		if (type == "v") {
			float x = 0, y = 0, z = 0;
			ss >> x >> y >> z;
			raw_positions.push_back(Vector3(x, y, z));
		}
		else if (type == "vt") {
			float x = 0, y = 0;
			ss >> x >> y;
			raw_texcoords.push_back(Vector2(x, y));
		}
		else if (type == "vn") {
			float x = 0, y = 0, z = 0;
			ss >> x >> y >> z;
			raw_normals.push_back(Vector3(x, y, z));
		}
		else if (type == "f") {
			std::vector<ObjIndices> faceVertices;
			std::string vertexStr;
			while (ss >> vertexStr) {
				ObjIndices idx = { -1, -1, -1 };
				std::replace(vertexStr.begin(), vertexStr.end(), '/', ' ');
				std::istringstream tokenStream(vertexStr);
				size_t first_slash = vertexStr.find(' ');
				size_t second_slash = vertexStr.find(' ', first_slash + 1);
				if (first_slash == std::string::npos) {
					tokenStream >> idx.vIdx;
				}
				else if (second_slash == first_slash + 1) {
					tokenStream >> idx.vIdx >> idx.vnIdx;
				}
				else {
					tokenStream >> idx.vIdx >> idx.vtIdx >> idx.vnIdx;
				}
				if (idx.vIdx > 0)  idx.vIdx -= 1;
				if (idx.vtIdx > 0) idx.vtIdx -= 1;
				if (idx.vnIdx > 0) idx.vnIdx -= 1;
				faceVertices.push_back(idx);
			}
			for (size_t i = 1; i + 1 < faceVertices.size(); ++i) {
				ObjIndices triangle[3] = { faceVertices[0], faceVertices[i], faceVertices[i + 1] };
				for (int j = 0; j < 3; ++j) {
					ObjIndices currentVertex = triangle[j];
					int foundIndex = -1;
					for (size_t k = 0; k < historyOfVertices.size(); ++k) {
						if (historyOfVertices[k].vIdx == currentVertex.vIdx &&
							historyOfVertices[k].vtIdx == currentVertex.vtIdx &&
							historyOfVertices[k].vnIdx == currentVertex.vnIdx) {
							foundIndex = static_cast<int>(k);
							break;
						}
					}
					if (foundIndex != -1) {
						ret->indexBuffer.push_back(foundIndex);
					}
					else {
						GLuint newIndex = static_cast<GLuint>(historyOfVertices.size());
						historyOfVertices.push_back(currentVertex);
						if (currentVertex.vIdx >= 0 && currentVertex.vIdx < (int)raw_positions.size()) {
							float x = raw_positions[currentVertex.vIdx].getX() + pos.getX();
							float y = raw_positions[currentVertex.vIdx].getX() + pos.getY();
							float z = raw_positions[currentVertex.vIdx].getZ() + pos.getZ();
							ret->vertexBuffer.push_back(x);
							ret->vertexBuffer.push_back(y);
							ret->vertexBuffer.push_back(z);
							if (x < minX) minX = x; if (x > maxX) maxX = x;
							if (y < minY) minY = y; if (y > maxY) maxY = y;
							if (z < minZ) minZ = z; if (z > maxZ) maxZ = z;
							hasVertices = true;
						}
						if (currentVertex.vtIdx >= 0 && currentVertex.vtIdx < (int)raw_texcoords.size()) {
							ret->texBuffer.push_back(raw_texcoords[currentVertex.vtIdx].getX());
							ret->texBuffer.push_back(raw_texcoords[currentVertex.vtIdx].getY());
						}
						else {
							ret->texBuffer.push_back(0.0f); ret->texBuffer.push_back(0.0f);
						}
						if (camera->getRenderUtils()->getLighting() != nullptr) {
							if (currentVertex.vnIdx >= 0 && currentVertex.vnIdx < (int)raw_normals.size()) {
								ret->normalBuffer.push_back(raw_normals[currentVertex.vnIdx].getX());
								ret->normalBuffer.push_back(raw_normals[currentVertex.vnIdx].getY());
								ret->normalBuffer.push_back(raw_normals[currentVertex.vnIdx].getZ());
							}
							else {
								ret->normalBuffer.push_back(0.0f); ret->normalBuffer.push_back(0.0f); ret->normalBuffer.push_back(1.0f);
							}
						}
						ret->indexBuffer.push_back(newIndex);
					}
				}
			}
		}
	}
	if (hasVertices) {
		ret->bounds = BoundingBox(Vector3(minX, minY, minZ), Vector3(maxX, maxY, maxZ));
	}
	else {
		ret->bounds = BoundingBox(pos, pos);
	}
	ret->setup();
	return ret;
}