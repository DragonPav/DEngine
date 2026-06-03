#include "Lighting.h"
#include <string>
#include <vector>
GLint maxTexUnits = 16;
DirLight::DirLight(Program* p) {
	this->p = p;
	ambient = Vector3(0.1f, 0.1f, 0.1f);
	ambient.uniformL = glGetUniformLocation(p->programId, "dirLight.ambient");
	diffuse = Vector3(0.6f, 0.6f, 0.6f);
	diffuse.uniformL = glGetUniformLocation(p->programId, "dirLight.diffuse");
	specular = Vector3(1, 1, 1);
	specular.uniformL = glGetUniformLocation(p->programId, "dirLight.specular");
	direction = Vector3(0, -2, -2);
	direction.uniformL = glGetUniformLocation(p->programId, "dirLight.direction");
	lightSpaceMatrix = Matrix4(glGetUniformLocation(p->programId, "lightSpaceMatrix"));
	shadowMapLoc = glGetUniformLocation(p->programId, "shadowMap");
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTexUnits);
	initShadows();
}
void DirLight::initShadows() {
	glGenFramebuffers(1, &depthFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, SHADOW_SIZE, SHADOW_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	const GLenum drawBuffers[] = { GL_NONE };
	glDrawBuffers(1, drawBuffers);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
	ambient.uniformL = glGetUniformLocation(p->programId, ambientS.c_str());
	diffuse = Vector3(0.6f, 0.6f, 0.6f);
	std::string diffuseS = "pointLights[" + std::to_string(number) + "].diffuse";
	diffuse.uniformL = glGetUniformLocation(p->programId, diffuseS.c_str());
	specular = Vector3(1, 1, 1);
	std::string specularS = "pointLights[" + std::to_string(number) + "].specular";
	specular.uniformL = glGetUniformLocation(p->programId, specularS.c_str());
	position = Vector3(0, 2, 0);
	std::string positionS = "pointLights[" + std::to_string(number) + "].position";
	position.uniformL = glGetUniformLocation(p->programId, positionS.c_str());
	std::string constantS = "pointLights[" + std::to_string(number) + "].constant";
	constantLoc = glGetUniformLocation(p->programId, constantS.c_str());
	std::string linearS = "pointLights[" + std::to_string(number) + "].linear";
	linearLoc = glGetUniformLocation(p->programId, linearS.c_str());
	std::string quadraticS = "pointLights[" + std::to_string(number) + "].quadratic";
	quadraticLoc = glGetUniformLocation(p->programId, quadraticS.c_str());
}
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
void Lighting::beginShadows(Program* shProg) {
	float halfBoxSize = 15.0f;
	glm::mat4 lightProjection = glm::ortho(-halfBoxSize, halfBoxSize, -halfBoxSize, halfBoxSize, 0.1f, 50.0f);
	glm::vec3 lightPos = glm::vec3(-dirLight.direction.x, -dirLight.direction.y, -dirLight.direction.z) * 10.0f;
	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	dirLight.lightSpaceMatrix.values = lightProjection * lightView;
	glBindFramebuffer(GL_FRAMEBUFFER, dirLight.depthFBO);
	glViewport(0, 0, dirLight.SHADOW_SIZE, dirLight.SHADOW_SIZE);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(shProg->programId);
	GLint mainPassMatrixLoc = dirLight.lightSpaceMatrix.uniformL;
	GLint shadowPassMatrixLoc = glGetUniformLocation(shProg->programId, "lightSpaceMatrix");
	if (shadowPassMatrixLoc != -1) {
		dirLight.lightSpaceMatrix.uniformL = shadowPassMatrixLoc;
		dirLight.lightSpaceMatrix.uniform();
	}
	dirLight.lightSpaceMatrix.uniformL = mainPassMatrixLoc;
}
void Lighting::endShadows(int width, int height, Program* mainProg) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	glUseProgram(mainProg->programId);
	glActiveTexture(GL_TEXTURE0 + (maxTexUnits - 1));
	glBindTexture(GL_TEXTURE_2D, dirLight.depthMap);;
}
Lighting::Lighting(Vector3 camPos, Program* p) : dirLight(p) {
	cameraPos = Vector3(camPos.x, camPos.y, camPos.z);
	cameraPos.uniformL = glGetUniformLocation(p->programId, "cameraPos");
	pointLights.push_back(PointLight(p, 0));
}
void Lighting::bind() {
	cameraPos.uniform();
	dirLight.bind();
	for (PointLight pointLight : pointLights) {
		pointLight.bind();
	}
}