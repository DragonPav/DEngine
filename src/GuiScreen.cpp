#include "GuiScreen.h"
GuiScreen::GuiScreen(RenderUtils* re, Input* inputSys, Vector2 viewport) {
	input = inputSys;
	guiCam = std::make_unique<Camera>(re);
	guiCam->setAutoUpdate(false);
	guiCam->ortho(0, viewport.getX(), viewport.getY(), 0, -1.0f, 1.0f);
	guiCam->setCameraPos(Vector3(0.0f, 0.0f, 1.0f));
	guiCam->lookAt(Vector3(0.0f, 0.0f, 0.0f));
	if (input != nullptr) {
		input->addMousePosCallback([this](double x, double y) {
			for (auto& btn : buttons) {
				btn->checkMousePosition(x, y);
			}
		});
		input->addMouseButtonCallback([this](int button, int action) {
			for (auto& btn : buttons) {
				btn->handleMouseClick(button, action);
			}
		});
	}
}
void GuiScreen::addElement(std::shared_ptr<GuiElement> element) {
	elements.push_back(element);
}
void GuiScreen::addButton(std::shared_ptr<GuiButton> button) {
	elements.push_back(button);
	buttons.push_back(button);
}

void GuiScreen::resize(int screenWidth, int screenHeight) {
    guiCam->ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight, -1.0f, 1.0f);
}
void GuiScreen::draw(Program* guiProgram) {
	if (elements.empty()) return;
	Program* oldProgram = guiCam->getRenderUtils()->getProgram();
	glDisable(GL_DEPTH_TEST);
	glUseProgram(guiProgram->getId());
	Matrix4& proj = guiCam->getProjection();
	proj.uniformL = glGetUniformLocation(guiProgram->getId(), "projection");
	proj.uniform();
	Matrix4& view = guiCam->getView();
	view.uniformL = glGetUniformLocation(guiProgram->getId(), "view");
	view.uniform();
	Lighting* bak = guiCam->getRenderUtils()->getLighting();
	guiCam->getRenderUtils()->setLighting(nullptr);
	for (std::shared_ptr<GuiElement> element : elements) {
		element->draw(guiProgram);
	}
	guiCam->getRenderUtils()->setLighting(bak);
	if (oldProgram != nullptr) {
		glUseProgram(oldProgram->getId());
	}
	glEnable(GL_DEPTH_TEST);
}