#pragma once
#include "QuadPent_Header.h"
#include "QuadPent_Font.h"

namespace QP {
	class Text {
	private:
		Font*                    FontPtr{};

		std::vector<wchar_t>     TextVec{};
		int                      PrevSize{};
		std::wstring             CurrentText{};
		std::wstring             PrevText{};

		Vector2                  RenderPosition{};
		Vector2                  CurrentRenderOffset{};
		float					 Rotation{};

		size_t                   TextWordCount{};
		float                    TextLength{};
		float                    TextRenderSize{};
		float                    TextLineGap{};
		float                    FixMiddleOffset{};

		std::vector<float>		 LineLengthList{};
		int                      CurrentLine{};
		int                      NumLine{};

		bool                     ShadowRenderState{};
		float					 TextOpacity{ 1.0f };
		Color3                   TextColor{ Color3(1.0, 1.0, 1.0) };
		Color3				     MacroColor{ Color3(1.0, 1.0, 1.0) };

		bool                     ShadowRenderCommand{};
		float					 ShadowOpacity{};
		Vector2                  ShadowOffset{};
		Color3                   ShadowColor{};

		Color3                   RenderColor{};
		float					 RenderOpacity{};

		int                      TextAlign{ ALIGN_DEFAULT };
		int                      RenderType{ RENDER_TYPE_STATIC };
		int                      HeightAlign{ HEIGHT_ALIGN_DEFAULT };
		bool                     FixMiddleCommand{};

	public:
		void Init(QP::Font& FontResource);
		void SetColor(float R, float G, float B);
		void SetColor(QP::Color3& Color);
		void SetColorRGB(int R, int G, int B);
		void SetMacroColor(QP::Color3& Color);
		void SetMacroColor(float R, float G, float B);
		void SetMacroColorRGB(int R, int G, int B);
		void SetAlign(int AlignOpt);
		void SetLineGap(float Value);
		void EnableFixMiddle();
		void DisableFixMiddle();
		void SetHeightAlign(int Type);
		void EnableShadow();
		void DisableShadow();
		void SetShadow(float OffsetX, float OffsetY, float Opacity, QP::Color3 Color = QP::Color3(0.0, 0.0, 0.0));
		void Rotate(float Degree);
		void SetOpacity(float Value);
		void SetRenderType(int Type);
		void Render(QP::Vector2& Position, float Size, const wchar_t* Fmt, ...);
		void Render(float X, float Y, float Size, const wchar_t* Fmt, ...);
		void RenderString(QP::Vector2& Position, float Size, std::string Str);
		void RenderString(float X, float Y, float Size, std::string Str);
		void RenderWString(QP::Vector2& Position, float Size, std::wstring Str);
		void RenderWString(float X, float Y, float Size, std::wstring Str);

	private:
		void ComputeTextLength(wchar_t*& Text);
		void InputText(std::vector<wchar_t>& Input, QP::Vector2& Position, float Size);
		void ProcessText(wchar_t* Text, QP::Vector2& Position, float Size);
		bool CheckColorMacro(wchar_t*& Text, int& Index);
		bool CheckCarrigeReturn(wchar_t*& Text, int& Index);
		void TransformText();
		void PrepareRender();
	};
}