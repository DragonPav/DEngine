#pragma once
#include "Lighting.h"
#include "ObjectBuffer.h"
#ifdef _WIN32
	#include <GLFW/glfw3.h>
#endif
class Lighting;
class RenderUtils {
private:
	#ifdef _WIN32
		GLFWwindow* window = nullptr;
	#else
		void* window = nullptr;
	#endif
	Lighting* lighting = nullptr;
	Program* p = nullptr;
public:
	ObjectBuffer createStaticVAO(GLfloat* vertices, GLuint* indices, GLfloat* tex, GLfloat* normal, GLuint vertexBufSize, GLuint indexBufSize, GLuint texBufSize, GLuint normalBufferSize);
	#ifdef _WIN32
		void setWindow(GLFWwindow* win);
	#else
		void setWindow(void* win) {};
	#endif
	/* @param `li` may be null, if you dont want use lighting, normals, etc. */
	void setLighting(Lighting* li);
	/* Sets program for some objects. */
	void setProgram(Program* prog);
	#ifdef _WIN32
		GLFWwindow* getWindow();
	#else
		void* getWindow() { return nullptr; };
	#endif
	/* @return If Lighting was set, then returns Ligthing*, else (and by the default) nullptr */
	Lighting* getLighting();
	Program* getProgram();
	RenderUtils() = delete;
	#ifdef _WIN32
		/* @param `program` Main Program.
		@param `li` Lighting*, can be null. 
		@param `window` Main window. */
		RenderUtils(Program* program, Lighting* li, GLFWwindow* window);
	#else
		/* @param `program` Main Program.
		@param `li` Lighting*, can be null. 
		@param `window` Main window. */
		RenderUtils(Program* program, Lighting* li, void* window);
	#endif
};