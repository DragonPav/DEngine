#include "Lighting.h"
#include <string>
#include <vector>
GLint maxTexUnits = 16;
DirLight::DirLight(Program* p) {
	this->p = p;
	ambient = Vector3(0.1f, 0.1f, 0.1f);
	ambient.setUniform(glGetUniformLocation(p->getId(), "dirLight.ambient"));
	diffuse = Vector3(0.6f, 0.6f, 0.6f);
	diffuse.setUniform(glGetUniformLocation(p->getId(), "dirLight.diffuse"));
	specular = Vector3(1, 1, 1);
	specular.setUniform(glGetUniformLocation(p->getId(), "dirLight.specular"));
	direction = Vector3(0, -2, -2);
	direction.setUniform(glGetUniformLocation(p->getId(), "dirLight.direction"));
	lightSpaceMatrix = Matrix4(glGetUniformLocation(p->getId(), "lightSpaceMatrix"));
	shadowMapLoc = glGetUniformLocation(p->getId(), "shadowMap");
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTexUnits);
	initShadows();
}
void DirLight::setProgram(Program* prog) {
	p = prog;
}
void DirLight::setAmbient(Vector3 a) {
	ambient = a;
};
void DirLight::setDiffuse(Vector3 d) {
	diffuse = d;
};
void DirLight::setSpecular(Vector3 s) {
	specular = s;
};
void DirLight::setDirection(Vector3 dir) {
	direction = dir;
};
Program* DirLight::getProgram() {
	return p;
};
Vector3& DirLight::getAmbient() {
	return ambient;
};
Vector3& DirLight::getDiffuse() {
	return diffuse;
};
Vector3& DirLight::getSpecular() {
	return specular;
};
Vector3& DirLight::getDirection() {
	return direction;
};
void DirLight::initShadows() {
	glGenFramebuffers(1, &depthFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_SIZE, SHADOW_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	#ifdef _WIN32
		glDrawBuffer(GL_NONE);
	#else
		GLenum noneBuffer[] = {GL_NONE};
		glDrawBuffers(1, noneBuffer);
	#endif
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void DirLight::dispose() {
	if (depthFBO != 0) {
		glDeleteFramebuffers(1, &depthFBO);
		depthFBO = 0;
	}
	if (depthMap != 0) {
		glDeleteTextures(1, &depthMap);
		depthMap = 0;
	}
}
void DirLight::bind() {
	ambient.uniform();
	diffuse.uniform();
	specular.uniform();
	direction.uniform();
	lightSpaceMatrix.uniform();
	if (shadowMapLoc != -1) {
		glUniform1i(shadowMapLoc, maxTexUnits - 1);
	}
}
PointLight::PointLight(Program* p, GLint number) {
	this->p = p;
	ambient = Vector3(0.1f, 0.1f, 0.1f);
	std::string ambientS = "pointLights[" + std::to_string(number) + "].ambient";
	ambient.setUniform(glGetUniformLocation(p->getId(), ambientS.c_str()));
	diffuse = Vector3(0.6f, 0.6f, 0.6f);
	std::string diffuseS = "pointLights[" + std::to_string(number) + "].diffuse";
	diffuse.setUniform(glGetUniformLocation(p->getId(), diffuseS.c_str()));
	specular = Vector3(1, 1, 1);
	std::string specularS = "pointLights[" + std::to_string(number) + "].specular";
	specular.setUniform(glGetUniformLocation(p->getId(), specularS.c_str()));
	position = Vector3(0, 2, 0);
	std::string positionS = "pointLights[" + std::to_string(number) + "].position";
	position.setUniform(glGetUniformLocation(p->getId(), positionS.c_str()));
	std::string constantS = "pointLights[" + std::to_string(number) + "].constant";
	constantLoc = glGetUniformLocation(p->getId(), constantS.c_str());
	std::string linearS = "pointLights[" + std::to_string(number) + "].linear";
	linearLoc = glGetUniformLocation(p->getId(), linearS.c_str());
	std::string quadraticS = "pointLights[" + std::to_string(number) + "].quadratic";
	quadraticLoc = glGetUniformLocation(p->getId(), quadraticS.c_str());
}
void PointLight::setProgram(Program* prog) {
	p = prog;
}
void PointLight::setAmbient(Vector3 a) {
	ambient = a;
};
void PointLight::setDiffuse(Vector3 d) {
	diffuse = d;
};
void PointLight::setSpecular(Vector3 s) {
	specular = s;
};
void PointLight::setPosition(Vector3 pos) {
	position = pos;
};
void PointLight::setConstantLocation(GLint loc) {
	constantLoc = loc;
}
void PointLight::setLinearLocation(GLint loc) {
	linearLoc = loc;
}
void PointLight::setQuadraticLocation(GLint loc) {
	quadraticLoc = loc;
}
Program* PointLight::getProgram() {
	return p;
};
Vector3& PointLight::getAmbient() {
	return ambient;
};
Vector3& PointLight::getDiffuse() {
	return diffuse;
};
Vector3& PointLight::getSpecular() {
	return specular;
};
Vector3& PointLight::getPosition() {
	return position;
};
GLint PointLight::getConstantLocation() {
	return constantLoc;
};
GLint PointLight::getLinearLocation() {
	return linearLoc;
};
GLint PointLight::getQuadraticLocation() {
	return quadraticLoc;
};
void PointLight::bind() {
	ambient.uniform();
	diffuse.uniform();
	specular.uniform();
	position.uniform();
	if (constantLoc != -1) {
		glUniform1f(constantLoc, constant);
	}
	if (linearLoc != -1) {
		glUniform1f(linearLoc, linear);
	}
	if (quadraticLoc != -1) {
		glUniform1f(quadraticLoc, quadratic);
	}
}
Lighting::~Lighting() {
	dispose();
}
void Lighting::dispose() {
	dirLight.dispose();
	for (PointLight* pointLight : pointLights) {
		if (pointLight != nullptr) {
			delete pointLight;
		}
	}
	pointLights.clear();
}
void Lighting::beginShadows(Program* shProg) {
	float halfBoxSize = 15.0f;
	glm::mat4 lightProjection = glm::ortho(-halfBoxSize, halfBoxSize, -halfBoxSize, halfBoxSize, 0.1f, 50.0f);
	glm::vec3 lightPos = glm::vec3(-dirLight.getDirection().getX(), -dirLight.getDirection().getY(), -dirLight.getDirection().getZ());
	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	dirLight.lightSpaceMatrix.values = lightProjection * lightView;
	glBindFramebuffer(GL_FRAMEBUFFER, dirLight.depthFBO);
	glViewport(0, 0, dirLight.SHADOW_SIZE, dirLight.SHADOW_SIZE);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(shProg->getId());
	GLint mainPassMatrixLoc = dirLight.lightSpaceMatrix.uniformL;
	if (shadowPassMatrixLoc == -1) {
		shadowPassMatrixLoc = glGetUniformLocation(shProg->getId(), "lightSpaceMatrix");
	}
	if (shadowPassMatrixLoc != -1) {
		dirLight.lightSpaceMatrix.uniformL = shadowPassMatrixLoc;
		dirLight.lightSpaceMatrix.uniform();
	}
	dirLight.lightSpaceMatrix.uniformL = mainPassMatrixLoc;
}
void Lighting::endShadows(int width, int height, Program* mainProg) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	glUseProgram(mainProg->getId());
	glActiveTexture(GL_TEXTURE0 + (maxTexUnits - 1));
	glBindTexture(GL_TEXTURE_2D, dirLight.depthMap);;
}
Lighting::Lighting(Vector3 camPos, Program* p) : dirLight(p) {
	cameraPos = Vector3(camPos.getX(), camPos.getY(), camPos.getZ());
	cameraPos.setUniform(glGetUniformLocation(p->getId(), "cameraPos"));
	pointLights.push_back(new PointLight(p, 0));
}
void Lighting::bind() {
	cameraPos.uniform();
	dirLight.bind();
	for (PointLight* pointLight : pointLights) {
		pointLight->bind();
	}
}