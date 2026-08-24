#pragma once
#include "Shader.h"

class Program {
public:
	Program() = delete;
	Program(Shader* vertex, Shader* fragment);
	~Program();
	void deleteProgram();
	bool link();
	GLuint getId();
private:
	Shader* v = nullptr;
	Shader* f = nullptr;
	GLuint programId = 0;
};