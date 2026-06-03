#include "RenderUtils.h"
RenderUtils::RenderUtils(Program* program, Lighting* li, int width, int height) {
	p = program;
	lighting = li;
	__android_log_write(ANDROID_LOG_INFO, "DEngine", "Initializing RenderUtils...");
	__android_log_write(ANDROID_LOG_INFO, "DEngine", "------------------------");
	std::string str = std::string("GPU: ") + reinterpret_cast<const char*>(glGetString(GL_RENDERER));
	__android_log_write(ANDROID_LOG_INFO, "DEngine", str.c_str());
	this->width = width;
	this->height = height;
	str = "Width: " + std::to_string(width);
	__android_log_write(ANDROID_LOG_INFO, "DEngine", str.c_str());
	str = "Height: " + std::to_string(height);
	__android_log_write(ANDROID_LOG_INFO, "DEngine", str.c_str());
}
ObjectBuffer RenderUtils::createStaticVAO(GLfloat* vertices, GLuint* indices, GLfloat* tex, GLfloat* normal, GLuint vertexBufSize, GLuint indexBufSize, GLuint texBufSize, GLuint normalBufSize) {
	ObjectBuffer b;
	glGenVertexArrays(1, &b.VAO);
	glBindVertexArray(b.VAO);
	glGenBuffers(1, &b.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, b.VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexBufSize, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glGenBuffers(1, &b.VBOtexture);
	glBindBuffer(GL_ARRAY_BUFFER, b.VBOtexture);
	glBufferData(GL_ARRAY_BUFFER, texBufSize, tex, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	if (lighting != nullptr) {
		glGenBuffers(1, &b.VBOnormal);
		glBindBuffer(GL_ARRAY_BUFFER, b.VBOnormal);
		glBufferData(GL_ARRAY_BUFFER, normalBufSize, normal, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(2);
	}
	glGenBuffers(1, &b.EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, b.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufSize, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return b;
}