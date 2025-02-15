#pragma once
#include "SDKHeader.h"
#include <vector>

#undef LoadImage

class ImageUtil {
private:
	GLuint                   VAO{};
	std::vector<PreLoadInfo> LoadBuffer{};
	std::vector<PreLoadSpriteSheetInfo> LoadSpriteSheetBuffer{};

	int ClipWidth{}, ClipHeight{};
	int NumRow{}, NumCol{}, BlankLocation{}, StartLocation{};

public:
	void Init();
	void LoadImage(Image& ImageStruct, std::string FilePath, int Type=IMAGE_TYPE_NEAREST);
	void LoadClip(Image& ImageStruct, std::string FilePath, int X, int Y, int ClipWidth, int ClipHeight, int Type=IMAGE_TYPE_NEAREST);
	void SetSpriteSheetSize(int ValueClipWidth, int ValueClipHeight, int ValueNumRow, int ValueNumCol, int ValueStartCol=1, int ValueNumBlank=0);
	void LoadSpriteSheet(SpriteSheet& SpriteSheetStruct, std::string FilePath, int Type=IMAGE_TYPE_NEAREST);
	void LoadSpriteSheetT(SpriteSheet& SpriteSheetStruct, std::string FilePath, int Type=IMAGE_TYPE_NEAREST);
	void LoadImageT(Image& ImageStruct, std::string FilePath, int Type=IMAGE_TYPE_NEAREST);
	void LoadClipT(Image& ImageStruct, std::string FilePath, int X, int Y, int ClipWidth, int ClipHeight, int Type=IMAGE_TYPE_NEAREST);
	void RenderImage(Image& ImageStruct, GLfloat OpacityValue=1.0, bool ApplyUnitTransform=false, bool DisableAdjustAspect=false);
	void RenderSpriteSheet(SpriteSheet& SpriteSheetStruct, GLfloat& Frame, GLfloat OpacityValue=1.0, bool ApplyUnitTransform=false, bool DisableAdjustAspect=false);
	void RenderStaticSpriteSheet(SpriteSheet& SpriteSheetStruct, int Frame, GLfloat OpacityValue=1.0, bool ApplyUnitTransform=false, bool DisableAdjustAspect=false);
	void RenderRaw();
	void UnmapImage(Image& ImageStruct);
	void UnmapSpriteSheet(SpriteSheet& SpriteSheetStruct);
	void Map();

	void Blur(GLfloat BlurStrength);
	void UnitBlur(GLfloat BlurStrength);
	void UnitOpacity(GLfloat Value);

	void SetColor(GLfloat R, GLfloat G, GLfloat B);
	void SetColor(glm::vec3& Color);
	void SetColorRGB(int R, int G, int B);

private:
	void PrepareRender(Image& ImageStruct);
	void PrepareRender(SpriteSheet& SpriteSheetStruct);
	void ProcessTransform(GLfloat Width, GLfloat Height, GLfloat OpacityValue, bool DisableAdjustAspect, bool ApplyUnitTransform);
};

namespace SDK {
	extern ImageUtil ImageTool;
}