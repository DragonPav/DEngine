#include "RenderUtils.h"
#ifdef _WIN32
	RenderUtils::RenderUtils(Program* program, Lighting* li, GLFWwindow* window) {
		p = program;
		lighting = li;
		this->window = window;
		//Some logging
		std::cout << "Initializing RenderUtils..." << std::endl;
		std::cout << "------------------------" << std::endl;
		std::cout << "GPU: " << glGetString(GL_RENDERER) << std::endl;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		std::cout << "Width: " << width << std::endl;
		std::cout << "Height: " << height << std::endl;
	}
#else
	RenderUtils::RenderUtils(Program* program, Lighting* li, void* window) {
		p = program;
		lighting = li;
		this->window = window;
		//Some logging
		std::cout << "Initializing RenderUtils..." << std::endl;
		std::cout << "------------------------" << std::endl;
		std::cout << "GPU: " << glGetString(GL_RENDERER) << std::endl;
	}
#endif
ObjectBuffer RenderUtils::createStaticVAO(GLfloat* vertices, GLuint* indices, GLfloat* tex, GLfloat* normal, GLuint vertexBufSize, GLuint indexBufSize, GLuint texBufSize, GLuint normalBufSize) {
	ObjectBuffer b;
	//VAO
	glGenVertexArrays(1, &b.VAO);
	glBindVertexArray(b.VAO);
	//VBO
	glGenBuffers(1, &b.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, b.VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexBufSize, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//Texture VBO
	glGenBuffers(1, &b.VBOtexture);
	glBindBuffer(GL_ARRAY_BUFFER, b.VBOtexture);
	glBufferData(GL_ARRAY_BUFFER, texBufSize, tex, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	//Normals, if lighting isn't using skipping
	if (lighting != nullptr) {
		glGenBuffers(1, &b.VBOnormal);
		glBindBuffer(GL_ARRAY_BUFFER, b.VBOnormal);
		glBufferData(GL_ARRAY_BUFFER, normalBufSize, normal, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(2);
	}
	//EBO
	glGenBuffers(1, &b.EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, b.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufSize, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return b;
}
#ifdef _WIN32
	void RenderUtils::setWindow(GLFWwindow* win) {
		if (window == nullptr) {
			throw std::runtime_error("RenderUtils::setWindow win is null");
			return;
		}
		window = win;
	}
#endif
void RenderUtils::setLighting(Lighting* li) {
	lighting = li;
}
void RenderUtils::setProgram(Program* prog) {
	if (p == nullptr) {
		throw std::runtime_error("RenderUtils::setProgram p is null");
		return;
	}
	p = prog;
}
#ifdef _WIN32
	GLFWwindow* RenderUtils::getWindow() {
		return window;
	}
#endif
Lighting* RenderUtils::getLighting() {
	return lighting;
}
Program* RenderUtils::getProgram() {
	return p;
}