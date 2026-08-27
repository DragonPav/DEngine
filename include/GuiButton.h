#include "GuiElement.h"
#include <functional>
#include "Input.h"
class GuiButton : public GuiElement {
private:
	std::function<void()> onClickCallback;
	bool isHovered = false;
	bool isPressed = false;
public:
	Texture* hoverTexture = nullptr;
	Texture* normalTexture = nullptr;
	Texture* pressedTexture = nullptr;
	GuiButton(Vector2 pos, Vector2 sz, Camera* guiCam, Texture* tex, std::function<void()> callback);
	void checkMousePosition(double mouseX, double mouseY);
	void handleMouseClick(int button, int action);
	void draw(Program* guiProgram) override;
};