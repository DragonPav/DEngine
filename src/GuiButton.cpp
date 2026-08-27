#include "GuiButton.h"
GuiButton::GuiButton(Vector2 pos, Vector2 sz, Camera* guiCam, Texture* normalTex, std::function<void()> callback) : GuiElement(pos, sz, guiCam, normalTex), onClickCallback(callback) {
	normalTexture = normalTex;
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
	if (isHovered && hoverTexture != nullptr) {
		if (isPressed && pressedTexture != nullptr) {
			textureRef = pressedTexture;
		} else {
			textureRef = hoverTexture;
		}
	} else {
		textureRef = normalTexture;
	}
	GuiElement::draw(guiProgram);
}