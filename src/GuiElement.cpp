#include "GuiElement.h"
GuiElement::GuiElement(Vector2 pos, Vector2 sz, Camera* guiCam, Texture* tex) {
	position = pos;
	size = sz;
	textureRef = tex;
	cameraRef = guiCam;
	float halfWidth = sz.getX() / 2.0f;
	float halfHeight = sz.getY() / 2.0f;
	GLfloat vertices[] = {
		-halfWidth, -halfHeight, 0,
		halfWidth, -halfHeight, 0,
		-halfWidth, halfHeight, 0,
		halfWidth, halfHeight, 0
	};
	GLuint indices[] = {
		0, 1, 2,
		1, 3, 2
	};
	GLfloat texCoords[] = {
		0, 0,
		1, 0,
		0, 1,
		1, 1
	};
	for (size_t i = 0; i < sizeof(vertices) / sizeof(GLfloat); i++) {
		vertexBuffer.push_back(vertices[i]);
	}
	for (size_t i = 0; i < sizeof(indices) / sizeof(GLuint); i++) {
		indexBuffer.push_back(indices[i]);
	}
	for (size_t i = 0; i < sizeof(texCoords) / sizeof(GLfloat); i++) {
		texBuffer.push_back(texCoords[i]);
	}
	vertexBufferSize = (GLuint) vertexBuffer.size() * sizeof(GLfloat);
	indexBufferSize = (GLuint) indexBuffer.size() * sizeof(GLuint);
	texBufferSize = (GLuint) texBuffer.size() * sizeof(GLfloat);
	Lighting* bak = guiCam->getRenderUtils()->getLighting();
	guiCam->getRenderUtils()->setLighting(nullptr);
	buffer = guiCam->getRenderUtils()->createStaticVAO(vertexBuffer.data(), indexBuffer.data(), texBuffer.data(), nullptr, nullptr, vertexBufferSize, indexBufferSize, texBufferSize, 0, 0);
	guiCam->getRenderUtils()->setLighting(bak);
	tintColor = Vector4(1, 1, 1, 1);
}
GuiElement::~GuiElement() {
	glDeleteVertexArrays(1, &buffer.VAO);
	std::vector<GLuint> buffersToDelete = {buffer.VBO, buffer.VBOtexture, buffer.EBO};
	glDeleteBuffers((GLuint) buffersToDelete.size(), buffersToDelete.data());
	buffer = ObjectBuffer();
}
void GuiElement::draw(Program* guiProgram) {
	if (guiProgram == nullptr) return;
	if (texSampleLoc == -1) texSampleLoc = glGetUniformLocation(guiProgram->getId(), "textureUnit");
	if (modelUniform == -1) modelUniform = glGetUniformLocation(guiProgram->getId(), "model");
	modelMatrix.uniformL = modelUniform;
	if (!matrixValueSet) {
		matrixValueSet = true;
		modelMatrix.values = glm::translate(glm::mat4(1.0f), glm::vec3(position.getX(), position.getY(), 0.0f));
	}
	modelMatrix.uniform();
	if (tintColorUniform == -1) tintColorUniform = glGetUniformLocation(guiProgram->getId(), "colorTint");
	tintColor.setUniform(tintColorUniform);
	tintColor.uniform();
	glActiveTexture(textureRef->getGLType());
	glBindTexture(GL_TEXTURE_2D, textureRef->getTextureId());
	if (texSampleLoc != -1) {
		glUniform1i(texSampleLoc, textureRef->getGLType() % GL_TEXTURE0);
	}
	glBindVertexArray(buffer.VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
Vector2& GuiElement::getPosition() {
	return position;
}
Vector2& GuiElement::getSize() {
	return size;
}
void GuiElement::setTintColor(Vector4 color) {
	tintColor = color;
}
Vector4& GuiElement::getTintColor() {
	return tintColor;
}
Matrix4& GuiElement::getModelMatrix() {
	return modelMatrix;
}