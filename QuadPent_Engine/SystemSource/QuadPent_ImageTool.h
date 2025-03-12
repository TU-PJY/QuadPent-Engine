#pragma once
#include "QuadPent_Header.h"

#undef LoadImage

namespace QP {
	class QuadPent_ImageTool {
	private:
		GLuint VAO{};
		std::vector<QP::ImageLoadBufferData> ImageLoadBuffer{};
		std::vector<QP::SpriteSheetLoadBufferData> SpriteSheetLoadBuffer{};

		glm::mat4 ImageAspectMatrix{ glm::mat4(1.0) };

		int ClipWidth{}, ClipHeight{};
		int NumRow{}, NumCol{}, BlankLocation{}, StartLocation{};

	public:
		void Init();
		void LoadImage(QP::Image& ImageStruct, std::string FilePath, int Type = IMAGE_TYPE_NEAREST);
		void LoadClip(QP::Image& ImageStruct, std::string FilePath, int X, int Y, int ClipWidth, int ClipHeight, int Type = IMAGE_TYPE_NEAREST);
		void SetSpriteSheetSize(int ValueClipWidth, int ValueClipHeight, int ValueNumRow, int ValueNumCol, int ValueStartCol = 1, int ValueNumBlank = 0);
		void LoadSpriteSheet(QP::SpriteSheet& SpriteSheetStruct, std::string FilePath, int Type = IMAGE_TYPE_NEAREST);
		void LoadSpriteSheetT(QP::SpriteSheet& SpriteSheetStruct, std::string FilePath, int Type = IMAGE_TYPE_NEAREST);
		void LoadImageT(QP::Image& ImageStruct, std::string FilePath, int Type = IMAGE_TYPE_NEAREST);
		void LoadClipT(QP::Image& ImageStruct, std::string FilePath, int X, int Y, int ClipWidth, int ClipHeight, int Type = IMAGE_TYPE_NEAREST);
		void RenderImage(QP::Image& ImageStruct, float OpacityValue = 1.0, bool ApplyGlobalAttribute = false, bool DisableAdjustAspect = false);
		void RenderSpriteSheet(QP::SpriteSheet& SpriteSheetStruct, float& Frame, float OpacityValue = 1.0, bool ApplyGlobalAttribute = false, bool DisableAdjustAspect = false);
		void RenderStaticSpriteSheet(QP::SpriteSheet& SpriteSheetStruct, int Frame, float OpacityValue = 1.0, bool ApplyGlobalAttribute = false, bool DisableAdjustAspect = false);
		void RenderRaw();
		void UnmapImage(QP::Image& ImageStruct);
		void UnmapSpriteSheet(QP::SpriteSheet& SpriteSheetStruct);
		void Map();

		void SetLocalBlur(float BlurStrength);
		void SetGlobalBlur(float BlurStrength);
		void SetGlobalOpacity(float Value);

		void SetGlobalColor(float R, float G, float B);
		void SetGlobalColor(QP::Color3& Color);
		void SetGlobalColorRGB(int R, int G, int B);

		void SetLocalColor(float R, float G, float B);
		void SetLocalColor(QP::Color3& Color);
		void SetLocalColorRGB(int R, int G, int B);

		QP::Vector2 RealSize(QP::Image& ImageStruct);
		QP::Vector2 RealSize(QP::SpriteSheet& SpriteShetStruct);

	private:
		void PrepareRender(QP::Image& ImageStruct);
		void PrepareRender(QP::SpriteSheet& SpriteSheetStruct);
		void ProcessTransform(float Width, float Height, float OpacityValue, bool ApplyGlobalAttribute, bool DisableAdjustAspect);
	};

	extern QP::QuadPent_ImageTool ImageTool;
}