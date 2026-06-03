#pragma once
#include "Shader.h"
#include <android/log.h>
class Program {
public:
	GLuint programId = 0;
	Program() = default;
	Program(Shader* vertex, Shader* fragment);
	void deleteProgram();
	bool link();
private:
	Shader* v = nullptr;
	Shader* f = nullptr;
};