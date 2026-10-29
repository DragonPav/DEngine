#pragma once
#include "ObjectBuffer.h"
#include "ObjectCreator.h"
#include "Texture.h"
#include "Vector4.h"
/** @class GuiElement
 * @brief Represents renderable GUI element
 */
class GuiElement {
protected:
	GuiElement() {};
	ObjectBuffer buffer;
	std::vector<GLfloat> vertexBuffer;
	std::vector<GLuint> indexBuffer;
	std::vector<GLfloat> texBuffer;
	GLuint vertexBufferSize = 0;
	GLuint indexBufferSize = 0;
	GLuint texBufferSize = 0;
	Matrix4 modelMatrix;
	Vector2 position;
	Vector2 size;
	Vector4 tintColor;
	Texture* textureRef = nullptr;
	Camera* cameraRef = nullptr;
	GLint texSampleLoc = -1;
	GLint modelUniform = -1;
	GLint tintColorUniform = -1;
	GLint isTextUniform = -1;
	bool matrixValueSet = false;
public:
	GuiElement(Vector2 pos, Vector2 sz, Camera* guiCam, Texture* tex);
	virtual ~GuiElement();
	/** @brief Draw `GuiElement` */
	virtual void draw(Program* guiProgram);
	/** @brief Get `GuiElement` position */
	Vector2& getPosition();
	/** @brief Get `GuiElement` size */
	Vector2& getSize();
	/** @brief Set `GuiElement` tint color */
	void setTintColor(Vector4 color);
	/** @brief Get `GuiElement` tint color */
	Vector4& getTintColor();
	/** @brief Get `GuiElement` matrix */
	Matrix4& getModelMatrix();
};