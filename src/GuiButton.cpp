#include "GuiButton.h"
GuiButton::GuiButton(Vector2 pos, Vector2 sz, Camera* guiCam, Texture* tex, std::function<void()> callback) : GuiElement(pos, sz, guiCam, tex), onClickCallback(callback) {
	texture = tex;
}
void GuiButton::checkMousePosition(double mouseX, double mouseY) {
	float halfW = size.getX() / 2.0f;
	float halfH = size.getY() / 2.0f;
	
	float minX = position.getX() - halfW;
	float maxX = position.getX() + halfW;
	float minY = position.getY() - halfH;
	float maxY = position.getY() + halfH;
	if (mouseX >= minX && mouseX <= maxX && mouseY >= minY && mouseY <= maxY) {
		isHovered = true;
	} else {
		isHovered = false;
	}
}
void GuiButton::handleMouseClick(int button, int action) {
	if (isHovered && button == 0 && action == Input::KEY_PRESSED) {
		isPressed = true;
		if (onClickCallback != nullptr) {
			onClickCallback();
		}
	} else if (button == 0 && action == Input::KEY_RELEASED) {
		isPressed = false;
	}
}
void GuiButton::draw(Program* guiProgram) {
	if (isTextUniform == -1) isTextUniform = glGetUniformLocation(guiProgram->getId(), "isText");
	if (isHovered) {
		if (isPressed) {
			tintColor.setX(0.7f);
			tintColor.setY(0.7f);
			tintColor.setZ(0.7f);
		} else {
			tintColor.setX(0.8f);
			tintColor.setY(0.8f);
			tintColor.setZ(0.8f);
		}
	} else {
		tintColor.setX(1.0f);
		tintColor.setY(1.0f);
		tintColor.setZ(1.0f);
	}
	if (isTextUniform != -1) glUniform1i(isTextUniform, 0);
	GuiElement::draw(guiProgram);
}