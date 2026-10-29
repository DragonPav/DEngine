#pragma once
#include "Vector3.h"
#include "Program.h"
#include "Object3D.h"
#include <vector>
#ifdef __ANDROID__
	#include <GLES3/gl32.h>
#else
	#include <GL/glew.h>
#endif
/** @class DirLight
 * @brief Directional light
 */
class DirLight {
private:
	Program* p;
	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;
	Vector3 direction;
	GLuint depthFBO;
	GLuint depthMap;
	const unsigned int SHADOW_SIZE = 2048;
	Matrix4 lightSpaceMatrix;
	GLint shadowMapLoc = -1;
	friend class Lighting;
public:
	DirLight() = delete;
	DirLight(Program* p);
	void setProgram(Program* prog);
	/** @brief Set ambient color */
	void setAmbient(Vector3 a);
	/** @brief Set diffuse/main color */
	void setDiffuse(Vector3 d);
	/** @brief Set specular color/force */
	void setSpecular(Vector3 s);
	/** @brief Set light direction */
	void setDirection(Vector3 dir);
	Program* getProgram();
	/** @brief Get ambient color */
	Vector3& getAmbient();
	/** @brief Get diffuse/main color */
	Vector3& getDiffuse();
	/** @brief Get specular color/force */
	Vector3& getSpecular();
	/** @brief Get light direction */
	Vector3& getDirection();
	/** @brief Internal use */
	void initShadows();
	/** @brief Destroy light */
	void dispose();
	/** @brief Internal use */
	void bind();
};
/** @class PointLight
 * @brief Point light
 */
class PointLight {
private:
	Program* p;
	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;
	Vector3 position;
	GLfloat constant = 1;
	GLfloat linear = 0.09f;
	GLfloat quadratic = 0.032f;
	GLint constantLoc = -1;
	GLint linearLoc = -1;
	GLint quadraticLoc = -1;
public:
	void setProgram(Program* prog);
	/** @brief Set ambient color */
	void setAmbient(Vector3 a);
	/** @brief Set diffuse/main color */
	void setDiffuse(Vector3 d);
	/** @brief Set specular color/force */
	void setSpecular(Vector3 s);
	/** @brief Set light position */
	void setPosition(Vector3 dir);
	/** @brief Set light constant location from shader */
	void setConstantLocation(GLint loc);
	/** @brief Set light linear location from shader */
	void setLinearLocation(GLint loc);
	/** @brief Set light quadratic location from shader */
	void setQuadraticLocation(GLint loc);
	Program* getProgram();
	/** @brief Get ambient color */
	Vector3& getAmbient();
	/** @brief Get main/diffuse color */
	Vector3& getDiffuse();
	/** @brief Get specular color/force */
	Vector3& getSpecular();
	/** @brief Get light position */
	Vector3& getPosition();
	/** @brief Get constant location from shader */
	GLint getConstantLocation();
	/** @brief Get linear location from shader */
	GLint getLinearLocation();
	/** @brief Get quadratic location from shader */
	GLint getQuadraticLocation();
	void bind();
	PointLight() = delete;
	PointLight(Program* p, GLint number);
};
/** @class Lighting
 * @brief Used for lighting render and settings
 */
class Lighting {
private:
	GLint shadowPassMatrixLoc = -1;
public:
	Vector3 cameraPos;
	DirLight dirLight;
	std::vector<PointLight> pointLights;
	GLint activeLightsLoc = -1;
	void bind();
	Lighting() = delete;
	Lighting(Vector3 camPos, Program* p);
	~Lighting();
	/** @brief Destroy `Lighting` */
	void dispose();
	/** @brief Should be called when shadow pass begins */
	void beginShadows(Program* shProg);
	/** @brief Should be called when shadow pass ends */
	void endShadows(int width, int height, Program* mainProg);
	/** @brief Add new point light to the `Lighting` */
	int addPointLight(Program* program, Vector3 position);
	/** @brief Remove point light from `Lighting` */
	void removePointLight(int index);
};