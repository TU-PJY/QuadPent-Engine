#pragma once
#include "EngineHeader.h"
#include <string>
#include <map>

enum class ImageType
{ Linear, Nearest };

class ImageUtil {
private:
	GLuint VAO{};

public:
	void Init();
	void ImportImage(unsigned int& Image, const char* FileName, ImageType Type);
	void Render(unsigned int ImageVar);
	void Release(unsigned int& ImageVar);
};
extern ImageUtil imageUtil;