#pragma once
#include "GuiButton.h"
#include <vector>
/** @class GuiScreen
 * @brief Holds any `GuiElement` and renders it
 */
class GuiScreen {
private:
	std::unique_ptr<Camera> guiCam = nullptr;
	std::vector<std::shared_ptr<GuiElement>> elements;
	std::vector<std::shared_ptr<GuiButton>> buttons;
	Input* input = nullptr;
public:
	GuiScreen(RenderUtils* re, Input* inputSys, Vector2 viewport);
	~GuiScreen() = default;
	/** @brief Add `GuiElement` to `GuiScreen` hierarchy */
	void addElement(std::shared_ptr<GuiElement> element);
	/** @brief Add `GuiButton` to `GuiScreen` hierarchy */
	void addButton(std::shared_ptr<GuiButton> button);
	/** Resize `GuiScreen` */
	void resize(int width, int height);
	/** Render `GuiScreen` with specified `Program` */
	void draw(Program* program);
};