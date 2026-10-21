#include "Object3D.h"
Object3D::Object3D(Camera* camera, RenderUtils* rend, Texture* tex) {
	if (camera == nullptr) {
		throw std::runtime_error("Camera is null");
	}
	if (rend == nullptr) {
		throw std::runtime_error("RenderUtils is null");
	}
	if (tex == nullptr) {
		throw std::runtime_error("Texture is null");
	}
	this->cam = camera;
	renderUtils = rend;
	texture = tex;
	model.values = glm::translate(glm::mat4(1.0f), glm::vec3(position.getX(), position.getY(), position.getZ()));
	modelLocation = glGetUniformLocation(rend->getProgram()->getId(), "model");
}
Object3D::~Object3D() {
	dispose();
}
void Object3D::setPosition(Vector3 pos) {
	position = pos;
}
void Object3D::setBounds(BoundingBox bb) {
	bounds = bb;
}
void Object3D::setTexture(Texture* tex) {
	texture = tex;
}
void Object3D::setCamera(Camera* camera) {
	cam = camera;
}
void Object3D::setPolygonMode(GLenum mode) {
	polygonMode = mode;
}
void Object3D::setNormalMap(Texture* tex) {
	tex->glType = GL_TEXTURE1;
	normalMap = tex;
}
void Object3D::calculateTangents(const std::vector<GLfloat>& vertices, const std::vector<GLfloat>& texCoords, const std::vector<GLfloat>& normals, const std::vector<GLuint>& indices, std::vector<GLfloat>& tangentBuffer) {
	size_t vertexCount = vertices.size() / 3;
    tangentBuffer.assign(vertexCount * 4, 0.0f);

    std::vector<glm::vec3> tan1(vertexCount, glm::vec3(0.0f));
    std::vector<glm::vec3> tan2(vertexCount, glm::vec3(0.0f));

    for (size_t i = 0; i < indices.size(); i += 3) {
        GLuint i1 = indices[i];
        GLuint i2 = indices[i + 1];
        GLuint i3 = indices[i + 2];

        glm::vec3 v1(vertices[i1 * 3], vertices[i1 * 3 + 1], vertices[i1 * 3 + 2]);
        glm::vec3 v2(vertices[i2 * 3], vertices[i2 * 3 + 1], vertices[i2 * 3 + 2]);
        glm::vec3 v3(vertices[i3 * 3], vertices[i3 * 3 + 1], vertices[i3 * 3 + 2]);

        glm::vec2 w1(texCoords[i1 * 2], texCoords[i1 * 2 + 1]);
        glm::vec2 w2(texCoords[i2 * 2], texCoords[i2 * 2 + 1]);
        glm::vec2 w3(texCoords[i3 * 2], texCoords[i3 * 2 + 1]);

        float x1 = v2.x - v1.x; float x2 = v3.x - v1.x;
        float y1 = v2.y - v1.y; float y2 = v3.y - v1.y;
        float z1 = v2.z - v1.z; float z2 = v3.z - v1.z;

        float s1 = w2.x - w1.x; float s2 = w3.x - w1.x;
        float t1 = w2.y - w1.y; float t2 = w3.y - w1.y;

        float r = 1.0f / (s1 * t2 - s2 * t1);
        glm::vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
        glm::vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);

        tan1[i1] += sdir; tan1[i2] += sdir; tan1[i3] += sdir;
        tan2[i1] += tdir; tan2[i2] += tdir; tan2[i3] += tdir;
    }

    for (size_t a = 0; a < vertexCount; a++) {
        glm::vec3 n(normals[a * 3], normals[a * 3 + 1], normals[a * 3 + 2]);
        glm::vec3 t = tan1[a];

        glm::vec3 tangent = glm::normalize(t - n * glm::dot(n, t));

        float w = (glm::dot(glm::cross(n, t), tan2[a]) < 0.0f) ? -1.0f : 1.0f;

        tangentBuffer[a * 4]     = tangent.x;
        tangentBuffer[a * 4 + 1] = tangent.y;
        tangentBuffer[a * 4 + 2] = tangent.z;
        tangentBuffer[a * 4 + 3] = w;
	}
}
Vector3& Object3D::getPosition() {
	return position;
}
BoundingBox& Object3D::getBounds() {
	return bounds;
}
Texture* Object3D::getTexture() {
	return texture;
}
Camera* Object3D::getCamera() {
	return cam;
}
GLenum Object3D::getPolygonMode() {
	return polygonMode;
}
Matrix4& Object3D::getModelMatrix() {
	return model;
}
void Object3D::setup() {
	GLuint verticesSize = (GLuint) vertexBuffer.size() * sizeof(GLfloat);
	GLuint texCoordsSize = (GLuint) texBuffer.size() * sizeof(GLfloat);
	GLuint indicesSize = (GLuint) indexBuffer.size() * sizeof(GLuint);
	GLuint normalsSize = (GLuint) normalBuffer.size() * sizeof(GLfloat);
	GLuint tangentsSize = 0;
	GLfloat* tangentsData = nullptr;
	if (normalMap != nullptr) {
		calculateTangents(vertexBuffer, texBuffer, normalBuffer, indexBuffer, tangentBuffer);
		tangentsSize = (GLuint) tangentBuffer.size() * sizeof(GLfloat);
		tangentsData = tangentBuffer.data();
	} else {
		tangentBuffer.clear();
	}
	b = renderUtils->createStaticVAO(vertexBuffer.data(), indexBuffer.data(), texBuffer.data(), normalBuffer.data(), tangentsData, verticesSize, indicesSize, texCoordsSize, normalsSize, tangentsSize);
}
void Object3D::begin() {
	glBindVertexArray(b.VAO);
}
void Object3D::render(Program* currentProgram, bool isShadowPass) {
	if (currentProgram == nullptr) return;

	if (!isShadowPass) {
		if (!cam->isBoxVisible(bounds)) {
			return;
		}
	}

	if (isShadowPass) {
		if (shadowModelLocation == -1) {
			shadowModelLocation = glGetUniformLocation(currentProgram->getId(), "model");
		}
		model.uniformL = shadowModelLocation;
		model.uniform();
	} else {
		model.uniformL = modelLocation;
		model.uniform();
	}

	if (isShadowPass) {
		glDrawElements(polygonMode, (GLuint)indexBuffer.size(), GL_UNSIGNED_INT, 0);
		return;
	}

	if (normalMatrixLocation == -1) {
		normalMatrixLocation = glGetUniformLocation(currentProgram->getId(), "normalMatrix");
	}
	if (useNormalMapLocation == -1) {
		useNormalMapLocation = glGetUniformLocation(currentProgram->getId(), "useNormalMap");
	}
	if (normalMatrixLocation != -1) {
		glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model.values)));
		glUniformMatrix3fv(normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}
	if (useNormalMapLocation != -1) {
		if (normalMap != nullptr) {
			glUniform1i(useNormalMapLocation, 1);
			glActiveTexture(normalMap->getGLType());
			glBindTexture(GL_TEXTURE_2D, normalMap->getTextureId());
			if (normalMapSampleLocation == -1) {
				normalMapSampleLocation = glGetUniformLocation(currentProgram->getId(), "normalMapUnit");
			}
			if (normalMapSampleLocation != -1) {
				glUniform1i(normalMapSampleLocation, normalMap->getGLType() - GL_TEXTURE0);
			}
		} else {
			glUniform1i(useNormalMapLocation, 0);
		}
	}
	glActiveTexture(texture->getGLType());
	glBindTexture(GL_TEXTURE_2D, texture->getTextureId());
	if (this->cam->isAutoUpdate()) {
		this->cam->update();
	}
	if (cam->getRenderUtils()->getLighting() != nullptr) {
		cam->getRenderUtils()->getLighting()->bind();
	}
	if (texture->getTextureSample() == -1) {
		std::cerr << "texture.texSampleNum is -1" << std::endl;
		return;
	}
	glUniform1i(texture->getTextureSample(), texture->getGLType() - GL_TEXTURE0);
	glDrawElements(polygonMode, (GLuint) indexBuffer.size(), GL_UNSIGNED_INT, 0);
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "OpenGL error occured: 0x" << std::hex << err << std::dec << std::endl;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	if (normalMap != nullptr) {
		glActiveTexture(normalMap->getGLType());
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glActiveTexture(GL_TEXTURE0);
}
void Object3D::dispose() {
	glDeleteVertexArrays(1, &b.VAO);
	std::vector<GLuint> buffersToDelete;
	buffersToDelete.push_back(b.VBO);
	buffersToDelete.push_back(b.VBOtexture);
	buffersToDelete.push_back(b.EBO);
	if (cam->getRenderUtils()->getLighting() != nullptr) buffersToDelete.push_back(b.VBOnormal);
	if (b.VBOtangent != 0) buffersToDelete.push_back(b.VBOtangent);
	glDeleteBuffers((GLuint) buffersToDelete.size(), buffersToDelete.data());
	b = ObjectBuffer();
}