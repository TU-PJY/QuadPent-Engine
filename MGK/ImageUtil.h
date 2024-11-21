#pragma once
#include "EngineHeader.h"
#include <vector>

class ImageUtil {
private:
	GLuint VAO{};
	std::vector<PreLoadInfo> LoadBuffer;

public:
	void Init();
	void Import(Image& ImageStruct, std::string FileName, int Type=IMAGE_TYPE_NEAREST);
	void PreLoad(Image& ImageStruct, std::string FileName, int Type=IMAGE_TYPE_NEAREST);
	void FinishLoad();
	void Render(Image& ImageStruct);
	void RenderRaw();
	void Release(Image& ImageStruct);
};
extern ImageUtil imageUtil;