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
	void ImportImage(Image& ImageStruct, const char* FileName, ImageType Type);
	void Render(Image& ImageStruct);
	void ReleaseImage(Image& ImageStruct);
};
extern ImageUtil imageUtil;