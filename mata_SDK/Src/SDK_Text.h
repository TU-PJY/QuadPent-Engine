#pragma once
#include "SDK_Header.h"
#include "SDK_Font.h"

namespace SDK {
	class Text {
	private:
		SDK::Font*               FontPtr{};

		std::vector<wchar_t>     TextVec{};
		int                      PrevSize{};
		std::wstring             CurrentText{};
		std::wstring             PrevText{};

		SDK::Vector2             RenderPosition{};
		SDK::Vector2             CurrentRenderOffset{};
		float					 Rotation{};

		size_t                   TextWordCount{};
		float                    TextLength{};
		float                    TextRenderSize{};
		float                    TextLineGap{};
		float                    FixMiddleOffset{};

		std::vector<float>		 LineLengthList{};
		int                      CurrentLine{};
		int                      NumLine{};

		float					 TextOpacity{ 1.0f };
		glm::vec3                TextColor{ glm::vec3(1.0, 1.0, 1.0) };

		bool                     ShadowRenderCommand{};
		float					 ShadowOpacity{};
		SDK::Vector2             ShadowOffset{};
		glm::vec3                ShadowColor{};

		glm::vec3                RenderColor{};
		float					 RenderOpacity{};

		int                      TextAlign{ ALIGN_DEFAULT };
		int                      RenderType{ RENDER_TYPE_STATIC };
		int                      HeightAlign{ HEIGHT_ALIGN_DEFAULT };
		bool                     FixMiddleCommand{};

	public:
		void Init(SDK::Font& FontResource);
		void SetColor(float R, float G, float B);
		void SetColor(SDK::Color3& Color);
		void SetColorRGB(int R, int G, int B);
		void SetAlign(int AlignOpt);
		void SetLineGap(float Value);
		void EnableFixMiddle();
		void DisableFixMiddle();
		void SetHeightAlign(int Type);
		void EnableShadow();
		void DisableShadow();
		void SetShadow(float OffsetX, float OffsetY, float Opacity, SDK::Color3 Color = SDK::Color3(0.0, 0.0, 0.0));
		void Rotate(float RotationValue);
		void SetOpacity(float Value);
		void SetRenderType(int Type);
		void Render(SDK::Vector2& Position, float Size, const wchar_t* Fmt, ...);
		void Render(float X, float Y, float Size, const wchar_t* Fmt, ...);
		void RenderString(SDK::Vector2& Position, float Size, std::string Str);
		void RenderString(float X, float Y, float Size, std::string Str);
		void RenderWString(SDK::Vector2& Position, float Size, std::wstring Str);
		void RenderWString(float X, float Y, float Size, std::wstring Str);

	private:
		void ComputeTextLength(const wchar_t* Text);
		void InputText(std::vector<wchar_t>& Input, SDK::Vector2& Position, float Size);
		void ProcessText(wchar_t* Text, SDK::Vector2& Position, float Size);
		void TransformText();
		void PrepareRender();
	};
}