#pragma once
#include "SDKHeader.h"
#include <vector>

class ImageUtil {
private:
	GLuint                   VAO{};
	std::vector<PreLoadInfo> LoadBuffer{};
	std::vector<PreLoadSpriteSheetInfo> LoadSpriteSheetBuffer{};

	int ClipWidth{}, ClipHeight{};
	int NumRow{}, NumCol{}, BlankLocation{}, StartLocation{};

public:
	void Init();
	void Load(Image& ImageStruct, std::string FilePath, int Type=IMAGE_TYPE_NEAREST);
	void LoadClip(Image& ImageStruct, std::string FilePath, int X, int Y, int ClipWidth, int ClipHeight, int Type=IMAGE_TYPE_NEAREST);
	void SetSpriteSheetSize(int ValueClipWidth, int ValueClipHeight, int ValueNumRow, int ValueNumCol, int ValueStartCol=1, int ValueNumBlank=0);
	void LoadSpriteSheet(SpriteSheet& SpriteSheetStruct, std::string FilePath, int Type=IMAGE_TYPE_NEAREST);
	void LoadSpriteSheetT(SpriteSheet& SpriteSheetStruct, std::string FilePath, int Type=IMAGE_TYPE_NEAREST);
	void LoadT(Image& ImageStruct, std::string FilePath, int Type=IMAGE_TYPE_NEAREST);
	void LoadClipT(Image& ImageStruct, std::string FilePath, int X, int Y, int ClipWidth, int ClipHeight, int Type=IMAGE_TYPE_NEAREST);
	void Map();
	void Render(Image& ImageStruct);
	void RenderSheet(SpriteSheet& SpriteSheetStruct, int Frame);
	void RenderRaw();
	void Release(Image& ImageStruct);
	void ReleaseSpriteSheet(SpriteSheet& SpriteSheetStruct);
};
extern ImageUtil imageUtil;