#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include "Object3D.h"
#include "Camera.h"
#include "Vector2.h"
/** @class ObjectCreator
 * @brief Used for create meshes
 */
class ObjectCreator {
private:
	static std::unique_ptr<Object3D> parseOBJ(std::stringstream& fileStream, Vector3 pos, Camera* cam, Texture* tex);
public:
	/** @brief Create custom mesh
	 * @throw std::runtime_error if null pointer
	 */
	static std::unique_ptr<Object3D> createCustom(std::vector<GLfloat> verts, std::vector<GLuint> indxs, std::vector<GLfloat> texCoords, std::vector<GLfloat> normals, Camera* camera, Texture* tex);
	/** @brief Create box
	 * @throw std::runtime_error if null pointer
	 */
	static std::unique_ptr<Object3D> createBox(Vector3 pos, Vector3 size, Camera* camera, Texture* tex);
	/** @brief Create quad (XY plane)
	 * @throw std::runtime_error if null pointer
	 */
	static std::unique_ptr<Object3D> createQuad(Vector3 pos, Vector2 size, Camera* camera, Texture* tex);
	/** @brief Create surface (XZ plane)
	 * @throw std::runtime_error if null pointer
	 */
	static std::unique_ptr<Object3D> createSurface(Vector3 pos, Vector2 size, Camera* camera, Texture* tex);
	/** @brief Create sphere
	 * @throw std::runtime_error if null pointer
	 */
	static std::unique_ptr<Object3D> createSphere(Vector3 pos, float radius, int sectorCount, int stackCount, Camera* camera, Texture* tex);
	/** @brief Create terrain
	 * @throw std::runtime_error if null pointer
	 */
	static std::unique_ptr<Object3D> createTerrain(Vector3 pos, int width, int height, float spacing, float scale, float heightScale, Camera* camera, Texture* tex);
	#ifdef __ANDROID__
		/** @brief Import OBJ file
	 	 * @throw std::runtime_error if null pointer
	 	 */
		static std::unique_ptr<Object3D> createFromOBJ(AAsset* file, Vector3 pos, Camera* camera, Texture* tex);
	#else
		/** @brief Import OBJ file
	 	 * @throw std::runtime_error if null pointer
	 	 */
		static std::unique_ptr<Object3D> createFromOBJ(std::ifstream* file, Vector3 pos, Camera* camera, Texture* tex);
	#endif
	ObjectCreator() = default;
};