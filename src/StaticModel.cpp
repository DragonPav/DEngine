#include "StaticModel.h"
StaticModel::StaticModel(Camera* camera, RenderUtils* rend, std::vector<Object3D>& objs) {
	if (camera == nullptr) {
		throw std::runtime_error("Camera is null");
	}
	if (rend == nullptr) {
		throw std::runtime_error("RenderUtils is null");
	}
	if (objs.empty()) {
		throw std::runtime_error("Objects is empty");
	}
	this->cam = camera;
	renderUtils = rend;
	texture = objs[0].getTexture();
	bool usesLightning = objs[0].getCamera()->getRenderUtils()->getLighting() != nullptr;

	size_t totalVertices = 0;
	size_t totalIndices = 0;
	size_t totalTexCoords = 0;
	size_t totalNormals = 0;

	for (auto& obj : objs) {
		totalVertices += obj.vertexBuffer.size();
		totalIndices += obj.indexBuffer.size();
		totalTexCoords += obj.texBuffer.size();
		totalNormals += obj.normalBuffer.size();
	}

	vertexBuffer.reserve(totalVertices);
	indexBuffer.reserve(totalIndices);
	texBuffer.reserve(totalTexCoords);
	if (usesLightning) {
		normalBuffer.reserve(totalNormals);
	}

	GLuint indexOffset = 0;
	for (const auto& obj : objs) {
		vertexBuffer.insert(vertexBuffer.end(), obj.vertexBuffer.begin(), obj.vertexBuffer.end());
		for (auto index : obj.indexBuffer) {
			indexBuffer.push_back(index + indexOffset);
		}
		texBuffer.insert(texBuffer.end(), obj.texBuffer.begin(), obj.texBuffer.end());
		if (usesLightning) {
			normalBuffer.insert(normalBuffer.end(), obj.normalBuffer.begin(), obj.normalBuffer.end());
		}
		indexOffset += (GLuint) obj.vertexBuffer.size() / 3;
	}

	vertBufSize = (GLuint) vertexBuffer.size() * sizeof(GLfloat);
	indexBufSize = (GLuint) indexBuffer.size() * sizeof(GLuint);
	texBufSize = (GLuint) texBuffer.size() * sizeof(GLfloat);
	normalBufSize = (GLuint) normalBuffer.size() * sizeof(GLfloat);
	model = Matrix4(glGetUniformLocation(rend->getProgram()->getId(), "model"));
	b = renderUtils->createStaticVAO(vertexBuffer.data(), indexBuffer.data(), texBuffer.data(), normalBuffer.data(), vertBufSize, indexBufSize, texBufSize, normalBufSize);
}
StaticModel::~StaticModel() {
	dispose();
}
void StaticModel::setTexture(Texture* tex) {
	texture = tex;
}
void StaticModel::setCamera(Camera* camera) {
	cam = camera;
}
Texture* StaticModel::getTexture() {
	return texture;
}
Camera* StaticModel::getCamera() {
	return cam;
}
Matrix4& StaticModel::getModelMatrix() {
	return model;
}
void StaticModel::begin() {
	model.values = glm::mat4(1.0f);
	glBindVertexArray(b.VAO);
}
void StaticModel::render(Program* currentProgram, bool isShadowPass) {
	if (currentProgram == nullptr) return;
	GLint currentModelLoc = glGetUniformLocation(currentProgram->getId(), "model");
	if (currentModelLoc != -1) {
		model.uniformL = currentModelLoc;
		model.uniform();
	}
	if (isShadowPass) {
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexBuffer.size()), GL_UNSIGNED_INT, 0);
		return;
	}
	glActiveTexture(texture->getGLType());
	glBindTexture(GL_TEXTURE_2D, texture->getTextureId());
	if (cam->isAutoUpdate()) {
		cam->update();
	}
	if (cam->getRenderUtils()->getLighting() != nullptr) {
		cam->getRenderUtils()->getLighting()->bind();
	}
	if (texture->getTextureSample() == -1) {
		std::cerr << "texture.texSampleNum is -1" << std::endl;
		return;
	}
	else {
		glUniform1i(texture->getTextureSample(), texture->getGLType() % GL_TEXTURE0);
	}
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexBuffer.size()), GL_UNSIGNED_INT, 0);
}
void StaticModel::dispose() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &b.VAO);
	std::vector<GLuint> buffersToDelete;
	buffersToDelete.push_back(b.VBO);
	buffersToDelete.push_back(b.VBOtexture);
	buffersToDelete.push_back(b.EBO);
	if (cam->getRenderUtils()->getLighting() != nullptr) buffersToDelete.push_back(b.VBOnormal);
	glDeleteBuffers(static_cast<GLsizei>(buffersToDelete.size()), buffersToDelete.data());
	b = ObjectBuffer();
}