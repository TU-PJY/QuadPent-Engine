#pragma once
#include "SDK_Header.h"
#include <vector>

#undef LoadImage

namespace MSDK {
	class SDK_ImageTool {
	private:
		GLuint                   VAO{};
		std::vector<MSDK::PreLoadInfo> LoadBuffer{};
		std::vector<MSDK::PreLoadSpriteSheetInfo> LoadSpriteSheetBuffer{};

		glm::mat4 ImageAspectMatrix{ glm::mat4(1.0) };

		int ClipWidth{}, ClipHeight{};
		int NumRow{}, NumCol{}, BlankLocation{}, StartLocation{};

	public:
		void Init();
		void LoadImage(MSDK::Image& ImageStruct, std::string FilePath, int Type = IMAGE_TYPE_NEAREST);
		void LoadClip(MSDK::Image& ImageStruct, std::string FilePath, int X, int Y, int ClipWidth, int ClipHeight, int Type = IMAGE_TYPE_NEAREST);
		void SetSpriteSheetSize(int ValueClipWidth, int ValueClipHeight, int ValueNumRow, int ValueNumCol, int ValueStartCol = 1, int ValueNumBlank = 0);
		void LoadSpriteSheet(MSDK::SpriteSheet& SpriteSheetStruct, std::string FilePath, int Type = IMAGE_TYPE_NEAREST);
		void LoadSpriteSheetT(MSDK::SpriteSheet& SpriteSheetStruct, std::string FilePath, int Type = IMAGE_TYPE_NEAREST);
		void LoadImageT(MSDK::Image& ImageStruct, std::string FilePath, int Type = IMAGE_TYPE_NEAREST);
		void LoadClipT(MSDK::Image& ImageStruct, std::string FilePath, int X, int Y, int ClipWidth, int ClipHeight, int Type = IMAGE_TYPE_NEAREST);
		void RenderImage(MSDK::Image& ImageStruct, float OpacityValue = 1.0, bool ApplyUnitTransform = false, bool DisableAdjustAspect = false);
		void RenderSpriteSheet(MSDK::SpriteSheet& SpriteSheetStruct, float& Frame, float OpacityValue = 1.0, bool ApplyUnitTransform = false, bool DisableAdjustAspect = false);
		void RenderStaticSpriteSheet(MSDK::SpriteSheet& SpriteSheetStruct, int Frame, float OpacityValue = 1.0, bool ApplyUnitTransform = false, bool DisableAdjustAspect = false);
		void RenderRaw();
		void UnmapImage(MSDK::Image& ImageStruct);
		void UnmapSpriteSheet(MSDK::SpriteSheet& SpriteSheetStruct);
		void Map();

		void Blur(float BlurStrength);
		void UnitBlur(float BlurStrength);
		void UnitOpacity(float Value);

		void SetColor(float R, float G, float B);
		void SetColor(glm::vec3& Color);
		void SetColorRGB(int R, int G, int B);

	private:
		void PrepareRender(MSDK::Image& ImageStruct);
		void PrepareRender(MSDK::SpriteSheet& SpriteSheetStruct);
		void ProcessTransform(float Width, float Height, float OpacityValue, bool DisableAdjustAspect, bool ApplyUnitTransform);
	};

	extern MSDK::SDK_ImageTool ImageTool;
}