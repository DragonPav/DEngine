#pragma once
#include "Lighting.h"
#include "ObjectBuffer.h"
#ifndef __ANDROID__
	#include <GLFW/glfw3.h>
#endif
class Lighting;
class RenderUtils {
private:
	#ifdef __ANDROID__
		void* window = nullptr;
	#else
		GLFWwindow* window = nullptr;
	#endif
	Lighting* lighting = nullptr;
	Program* p = nullptr;
public:
	ObjectBuffer createStaticVAO(GLfloat* vertices, GLuint* indices, GLfloat* tex, GLfloat* normal, GLuint vertexBufSize, GLuint indexBufSize, GLuint texBufSize, GLuint normalBufferSize);
	#ifdef __ANDROID__
		void setWindow(void* win) {};
	#else
		void setWindow(GLFWwindow* win);
	#endif
	/* @param `li` may be null, if you dont want use lighting, normals, etc. */
	void setLighting(Lighting* li);
	/* Sets program for some objects. */
	void setProgram(Program* prog);
	#ifdef __ANDROID__
		void* getWindow() { return nullptr; };
	#else
		GLFWwindow* getWindow();
	#endif
	/* @return If Lighting was set, then returns Ligthing*, else (and by the default) nullptr */
	Lighting* getLighting();
	Program* getProgram();
	RenderUtils() = delete;
	#ifdef __ANDROID__
		/* @param `program` Main Program.
		@param `li` Lighting*, can be null. 
		@param `window` Main window. */
		RenderUtils(Program* program, Lighting* li, void* window);
	#else
		/* @param `program` Main Program.
		@param `li` Lighting*, can be null. 
		@param `window` Main window. */
		RenderUtils(Program* program, Lighting* li, GLFWwindow* window);
	#endif
};