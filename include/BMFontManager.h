#pragma once
#include "Texture.h"
#include "Vector4.h"
#include <sstream>
#include <memory>
/** @struct BMFontChar
 * @brief Font char data
 */
struct BMFontChar {
	int id = 32, x, y, width, height, xoffset, yoffset, xadvance;
};
/** @class BMFontManager
 * @brief Generates AngelCode font description
 */
class BMFontManager {
private:
	Texture::Config pageCfg;
	std::unique_ptr<Texture> page = nullptr;
	BMFontChar* chars = nullptr;
	int scaleW = 0;
	int scaleH = 0;
	int charCount = 0;
	void parseFile(std::stringstream& strStream);
public:
	#ifdef __ANDROID__
		BMFontManager(AAsset* fntFile, AAsset* pngFile, Program* program);
	#else
		BMFontManager(const char* fntFile, const char* pngFile, Program* program);
	#endif
	int getScaleW();
	int getScaleH();
	int getCharCount();
	Texture* getPage();
	BMFontChar getChar(int which);
	~BMFontManager();
};