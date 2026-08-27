#include "ObjectBuffer.h"
#include "ObjectCreator.h"
#include "Texture.h"
#include <memory>
class GuiElement {
protected:
	ObjectBuffer buffer;
	std::vector<GLfloat> vertexBuffer;
	std::vector<GLuint> indexBuffer;
	std::vector<GLfloat> texBuffer;
	GLuint vertexBufferSize = 0;
	GLuint indexBufferSize = 0;
	GLuint texBufferSize = 0;
	Vector2 position;
	Vector2 size;
	Texture* textureRef = nullptr;
	Camera* cameraRef = nullptr;
public:
	GuiElement(Vector2 pos, Vector2 sz, Camera* guiCam, Texture* tex);
	virtual ~GuiElement();
	virtual void draw(Program* guiProgram);
	Vector2& getPosition();
	Vector2& getSize();
};