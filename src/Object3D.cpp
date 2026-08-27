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
	b = renderUtils->createStaticVAO(vertexBuffer.data(), indexBuffer.data(), texBuffer.data(), normalBuffer.data(), verticesSize, indicesSize, texCoordsSize, normalsSize);
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

	if (normalMatrixLocation != -1) {
		glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model.values)));
		glUniformMatrix3fv(normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));
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
	glUniform1i(texture->getTextureSample(), texture->getGLType() % GL_TEXTURE0);
	glDrawElements(polygonMode, (GLuint) indexBuffer.size(), GL_UNSIGNED_INT, 0);
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "OpenGL error occured: 0x" << std::hex << err << std::dec << std::endl;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void Object3D::dispose() {
	glDeleteVertexArrays(1, &b.VAO);
	std::vector<GLuint> buffersToDelete;
	buffersToDelete.push_back(b.VBO);
	buffersToDelete.push_back(b.VBOtexture);
	buffersToDelete.push_back(b.EBO);
	if (cam->getRenderUtils()->getLighting() != nullptr) buffersToDelete.push_back(b.VBOnormal);
	glDeleteBuffers((GLuint) buffersToDelete.size(), buffersToDelete.data());
	b = ObjectBuffer();
}