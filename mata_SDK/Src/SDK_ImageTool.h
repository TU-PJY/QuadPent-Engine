#pragma once
#include "SDK_Header.h"
#include <vector>

#undef LoadImage

namespace SDK {
	class SDK_ImageTool {
	private:
		GLuint                   VAO{};
		std::vector<SDK::PreLoadInfo> LoadBuffer{};
		std::vector<SDK::PreLoadSpriteSheetInfo> LoadSpriteSheetBuffer{};

		int ClipWidth{}, ClipHeight{};
		int NumRow{}, NumCol{}, BlankLocation{}, StartLocation{};

	public:
		void Init();
		void LoadImage(SDK::Image& ImageStruct, std::string FilePath, int Type = IMAGE_TYPE_NEAREST);
		void LoadClip(SDK::Image& ImageStruct, std::string FilePath, int X, int Y, int ClipWidth, int ClipHeight, int Type = IMAGE_TYPE_NEAREST);
		void SetSpriteSheetSize(int ValueClipWidth, int ValueClipHeight, int ValueNumRow, int ValueNumCol, int ValueStartCol = 1, int ValueNumBlank = 0);
		void LoadSpriteSheet(SDK::SpriteSheet& SpriteSheetStruct, std::string FilePath, int Type = IMAGE_TYPE_NEAREST);
		void LoadSpriteSheetT(SDK::SpriteSheet& SpriteSheetStruct, std::string FilePath, int Type = IMAGE_TYPE_NEAREST);
		void LoadImageT(SDK::Image& ImageStruct, std::string FilePath, int Type = IMAGE_TYPE_NEAREST);
		void LoadClipT(SDK::Image& ImageStruct, std::string FilePath, int X, int Y, int ClipWidth, int ClipHeight, int Type = IMAGE_TYPE_NEAREST);
		void RenderImage(SDK::Image& ImageStruct, GLfloat OpacityValue = 1.0, bool ApplyUnitTransform = false, bool DisableAdjustAspect = false);
		void RenderSpriteSheet(SDK::SpriteSheet& SpriteSheetStruct, GLfloat& Frame, GLfloat OpacityValue = 1.0, bool ApplyUnitTransform = false, bool DisableAdjustAspect = false);
		void RenderStaticSpriteSheet(SDK::SpriteSheet& SpriteSheetStruct, int Frame, GLfloat OpacityValue = 1.0, bool ApplyUnitTransform = false, bool DisableAdjustAspect = false);
		void RenderRaw();
		void UnmapImage(SDK::Image& ImageStruct);
		void UnmapSpriteSheet(SDK::SpriteSheet& SpriteSheetStruct);
		void Map();

		void Blur(GLfloat BlurStrength);
		void UnitBlur(GLfloat BlurStrength);
		void UnitOpacity(GLfloat Value);

		void SetColor(GLfloat R, GLfloat G, GLfloat B);
		void SetColor(glm::vec3& Color);
		void SetColorRGB(int R, int G, int B);

	private:
		void PrepareRender(SDK::Image& ImageStruct);
		void PrepareRender(SDK::SpriteSheet& SpriteSheetStruct);
		void ProcessTransform(GLfloat Width, GLfloat Height, GLfloat OpacityValue, bool DisableAdjustAspect, bool ApplyUnitTransform);
	};
}

namespace SDK {
	extern SDK::SDK_ImageTool ImageTool;
}