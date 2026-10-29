#include "GuiText.h"
void GuiText::updateText() {
	if (manager != nullptr) {
		textureRef = manager->getPage();
	}
	if (text.empty()) return;
	float cursorX = 0;
	float cursorY = 0;
	GLuint vertexCount = 0;
	int scaleW = manager->getScaleW();
	int scaleH = manager->getScaleH();
	for (wchar_t symbol : text) {
		int unicode_id = static_cast<int>(symbol);
		BMFontChar glyph = manager->getChar(unicode_id);
		GLfloat x0 = cursorX + glyph.xoffset;
		GLfloat y0 = cursorY + glyph.yoffset;
		GLfloat x1 = x0 + glyph.width;
		GLfloat y1 = y0 + glyph.height;
		GLfloat s0 = (GLfloat) glyph.x / scaleW;
		GLfloat t0 = (GLfloat) glyph.y / scaleH;
		GLfloat s1 = (GLfloat) (glyph.x + glyph.width) / scaleW;
		GLfloat t1 = (GLfloat) (glyph.y + glyph.height) / scaleH;
		// Buffers
		GLfloat vertices[] = {
			x0, y0, 0,
			x1, y0, 0,
			x0, y1, 0,
			x1, y1, 0
		};
		GLfloat texCoords[] = {
			s0, t0,
			s1, t0,
			s0, t1,
			s1, t1
		};
		GLuint indices[] = {
			vertexCount, vertexCount + 1, vertexCount + 2,
			vertexCount + 1, vertexCount + 3, vertexCount + 2,
		};
		for (int i = 0; i < sizeof(vertices) / sizeof(GLfloat); i++) {
			vertexBuffer.push_back(vertices[i]);
		}
		for (int i = 0; i < sizeof(texCoords) / sizeof(GLfloat); i++) {
			texBuffer.push_back(texCoords[i]);
		}
		for (int i = 0; i < sizeof(indices) / sizeof(GLuint); i++) {
			indexBuffer.push_back(indices[i]);
		}
		vertexCount += 4;
		cursorX += glyph.xadvance;
	}
	vertexBufferSize = vertexBuffer.size() * sizeof(GLfloat);
	texBufferSize = texBuffer.size() * sizeof(GLfloat);
	indexBufferSize = indexBuffer.size() * sizeof(GLuint);
	if (buffer.VAO != 0) {
		glDeleteVertexArrays(1, &buffer.VAO);
		std::vector<GLuint> buffersToDelete = {buffer.VBO, buffer.VBOtexture, buffer.EBO};
		glDeleteBuffers((GLuint) buffersToDelete.size(), buffersToDelete.data());
	}
	Lighting* bak = cameraRef->getRenderUtils()->getLighting();
	cameraRef->getRenderUtils()->setLighting(nullptr);
	buffer = cameraRef->getRenderUtils()->createStaticVAO(vertexBuffer.data(), indexBuffer.data(), texBuffer.data(), nullptr, nullptr,
		vertexBufferSize, indexBufferSize, texBufferSize, 0, 0);
	cameraRef->getRenderUtils()->setLighting(bak);
	textureRef = manager->getPage();
}
GuiText::GuiText(std::wstring text, Vector2 pos, Vector2 sz, Camera* guiCam, BMFontManager* mgr) : GuiElement() {
	this->text = text;
	position = pos;
	size = sz;
	manager = mgr;
	cameraRef = guiCam;
	updateText();
}
void GuiText::draw(Program* guiProgram) {
	if (guiProgram == nullptr) return;
	if (texSampleLoc == -1) texSampleLoc = glGetUniformLocation(guiProgram->getId(), "textureUnit");
	if (modelUniform == -1) modelUniform = glGetUniformLocation(guiProgram->getId(), "model");
	if (isTextUniform == -1) isTextUniform = glGetUniformLocation(guiProgram->getId(), "isText");
	modelMatrix.uniformL = modelUniform;
	if (!matrixValueSet) {
		matrixValueSet = true;
		modelMatrix.values = glm::translate(glm::mat4(1.0f), glm::vec3(position.getX(), position.getY(), 0.0f));
		modelMatrix.values = glm::scale(modelMatrix.values, glm::vec3(size.getX(), size.getY(), 1.0f));
	}
	modelMatrix.uniform();
	if (tintColorUniform == -1) tintColorUniform = glGetUniformLocation(guiProgram->getId(), "colorTint");
	tintColor.setUniform(tintColorUniform);
	tintColor.uniform();
	if (isTextUniform != -1) glUniform1i(isTextUniform, 1);
	glActiveTexture(textureRef->getGLType());
	glBindTexture(GL_TEXTURE_2D, textureRef->getTextureId());
	if (texSampleLoc != -1) {
		glUniform1i(texSampleLoc, textureRef->getGLType() % GL_TEXTURE0);
	}
	glBindVertexArray(buffer.VAO);
	glDrawElements(GL_TRIANGLES, (GLsizei)indexBuffer.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}