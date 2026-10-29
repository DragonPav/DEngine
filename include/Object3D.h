#pragma once
#include <vector>
#include "Texture.h"
#include "Camera.h"
#include "ObjectBuffer.h"
#include "BoundingBox.h"
class Camera;
class RenderUtils;
/** @class Object3D
 * @brief Used for renderable object
 */
class Object3D {
private:
	Vector3 position;
	Matrix4 model;
	BoundingBox bounds;
	std::vector<GLfloat> vertexBuffer;
	std::vector<GLuint> indexBuffer;
	std::vector<GLfloat> texBuffer;
	std::vector<GLfloat> normalBuffer;
	std::vector<GLfloat> tangentBuffer;
	Texture* texture;
	ObjectBuffer b;
	Camera* cam = nullptr;
	Texture* normalMap = nullptr;
	RenderUtils* renderUtils = nullptr;
	GLenum polygonMode = GL_TRIANGLES;
	GLint shadowModelLocation = -1;
	GLint modelLocation = -1;
	GLint normalMatrixLocation = -1;
	GLint useNormalMapLocation = -1;
	GLint normalMapSampleLocation = -1;
	friend class StaticModel;
	friend class ObjectCreator;
public:
	void setPosition(Vector3 pos);
	void setBounds(BoundingBox bb);
	void setTexture(Texture* tex);
	void setCamera(Camera* camera);
	void setPolygonMode(GLenum mode);
	void setNormalMap(Texture* tex);
	/** @brief Calculate tangents to given tangent buffer */
	void calculateTangents(const std::vector<GLfloat>& vertices, const std::vector<GLfloat>& texCoords, const std::vector<GLfloat>& normals, const std::vector<GLuint>& indices, std::vector<GLfloat>& tangentBuffer);
	Vector3& getPosition();
	BoundingBox& getBounds();
	Texture* getTexture();
	Camera* getCamera();
	GLenum getPolygonMode();
	Matrix4& getModelMatrix();
	void setup();
	void begin();
	void render(Program* currentProgram, bool isShadowPass);
	/** @brief Destroy `Object3D` */
	void dispose();
	Object3D() = delete;
	Object3D(Camera* camera, RenderUtils* rend, Texture* tex);
	~Object3D();
};