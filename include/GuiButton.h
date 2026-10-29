#pragma once
#include "GuiElement.h"
#include <functional>
#include "Input.h"
/** @class GuiButton
 * @brief `GuiElement` which can be interacted with user
 */
class GuiButton : public GuiElement {
private:
	std::function<void()> onClickCallback;
	bool isHovered = false;
	bool isPressed = false;
public:
	Texture* texture = nullptr;
	GuiButton(Vector2 pos, Vector2 sz, Camera* guiCam, Texture* tex, std::function<void()> callback);
	/** @brief Check if mouse hover button, then `isHovered` will be set to `true`, `false` otherwise */
	void checkMousePosition(double mouseX, double mouseY);
	/** @brief `Input` mouse callback handler */
	void handleMouseClick(int button, int action);
	/** @brief Draw element */
	virtual void draw(Program* guiProgram) override;
};