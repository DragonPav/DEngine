#include "GuiButton.h"
#include <vector>
class GuiScreen {
private:
	std::unique_ptr<Camera> guiCam = nullptr;
	std::vector<std::shared_ptr<GuiElement>> elements;
	std::vector<std::shared_ptr<GuiButton>> buttons;
	Input* input = nullptr;
public:
	GuiScreen(RenderUtils* re, Input* inputSys, Vector2 viewport);
	~GuiScreen() = default;
	void addElement(std::shared_ptr<GuiElement> element);
	void addButton(std::shared_ptr<GuiButton> button);
	void resize(int width, int height);
	void draw(Program* program);
};