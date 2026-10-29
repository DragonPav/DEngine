#pragma once
#include "GuiElement.h"
#include "BMFontManager.h"
/** @class GuiText
 * @brief Used for render Fonts in GUI
 */
class GuiText : public GuiElement {
private:
	std::wstring text;
	BMFontManager* manager = nullptr;
	void updateText();
public:
	/** @param sz Scale value e.g. 0.7, 0.2 */
	GuiText(std::wstring text, Vector2 pos, Vector2 sz, Camera* guiCam, BMFontManager* mgr);
	virtual void draw(Program* guiProgram) override;
};