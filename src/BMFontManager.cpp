#include "BMFontManager.h"
#include "Logger.h"
void BMFontManager::parseFile(std::stringstream& strStream) {
	int charNum = 0;
	std::string line;
	while (std::getline(strStream, line)) {
		std::istringstream ss(line);
		std::string type;
		ss >> type;
		if (type == "common") {
			std::string token;
			while (ss >> token) {
				size_t eqPos = token.find('=');
				if (eqPos != std::string::npos) {
					std::string key = token.substr(0, eqPos);
					std::string value = token.substr(eqPos + 1);
					if (key == "scaleW") {
						scaleW = std::stoi(value);
					} else if (key == "scaleH") {
						scaleH = std::stoi(value);
					}
				}
			}
		} else if (type == "chars") {
			std::string token;
			ss >> token;
			size_t eqPos = token.find('=');
			if (eqPos != std::string::npos) {
				std::string value = token.substr(eqPos + 1);
				charCount = std::stoi(value);
				chars = new BMFontChar[charCount];
			}
		} else if (type == "char") {
			BMFontChar newChar;
			while (ss >> type) {
				size_t eqPos = type.find('=');
				if (eqPos != std::string::npos) {
					std::string key = type.substr(0, eqPos);
					std::string value = type.substr(eqPos + 1);
					if (key == "id") newChar.id = std::stoi(value);
					else if (key == "x") newChar.x = std::stoi(value);
					else if (key == "y") newChar.y = std::stoi(value);
					else if (key == "width") newChar.width = std::stoi(value);
					else if (key == "height") newChar.height = std::stoi(value);
					else if (key == "xoffset") newChar.xoffset = std::stoi(value);
					else if (key == "yoffset") newChar.yoffset = std::stoi(value);
					else if (key == "xadvance") newChar.xadvance = std::stoi(value);
				}
			}
			chars[charNum] = newChar;
			charNum ++;
		}
	}
}
#ifdef __ANDROID__
	BMFontManager::BMFontManager(AAsset* fntFile, AAsset* pngFile, Program* program) {
		off_t fntfileSize = AAsset_getLength(fntFile);
		std::string content;
		content.resize(fntfileSize);
		int bytesRead = AAsset_read(fntFile, &content[0], fntfileSize);
		if (bytesRead < 0) {
			Logger::print(Logger::Error, "[BMFontManager]: Failed to read AAsset");
		}
		std::stringstream strStream(content);
		parseFile(strStream);
		AAsset_close(fntFile);
		// TEXTURE
		pageCfg.useMipmap = false;
		pageCfg.textureMinFilter = GL_LINEAR;
		pageCfg.textureMagFilter = GL_LINEAR;
		page = std::make_unique<Texture>(program, pngFile, pageCfg);
	}
#else
	BMFontManager::BMFontManager(const char* fntFile, const char* pngFile, Program* program) {
		std::ifstream fntFileStr(fntFile);
		if (!fntFileStr.is_open()) {
			Logger::print(Logger::Error, "[BMFontManager]: Error to open file: " + std::string(fntFile));
			return;
		}
		std::stringstream strStream;
		strStream << fntFileStr.rdbuf();
		parseFile(strStream);
		fntFileStr.close();
		// TEXTURE
		pageCfg.useMipmap = false;
		pageCfg.textureMinFilter = GL_LINEAR;
		pageCfg.textureMagFilter = GL_LINEAR;
		page = std::make_unique<Texture>(program, std::string(pngFile), pageCfg);
	}
#endif
BMFontManager::~BMFontManager() {
	page.reset();
	delete[] chars;
}
int BMFontManager::getScaleW() {
	return scaleW;
}
int BMFontManager::getScaleH() {
	return scaleH;
}
Texture* BMFontManager::getPage() {
	return page.get();
}
BMFontChar BMFontManager::getChar(int which) {
	for (int i = 0; i < charCount; i++) {
		if (chars[i].id == which) {
			return chars[i];
		}
	}
	return BMFontChar{};
}