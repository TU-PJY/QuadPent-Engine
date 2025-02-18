#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "SDK_Header.h"
#include <windows.h>
#include <map>
#include <unordered_map>

namespace MSDK {
	class Text {
	private:
		glm::vec2                RenderPosition{};
		glm::vec2                CurrentRenderOffset{};
		float                  Rotation{};

		size_t                   TextWordCount{};
		float                  TextLength{};
		float                  TextRenderSize{};
		float                  TextLineGap{};
		float                  MiddleHeight{};

		std::vector<float>     LineLengthBuffer{};
		std::wstring             CurrentText{};
		std::wstring             PrevText{};
		int                      CurrentLine{};

		float                  TextOpacity{ 1.0f };
		glm::vec3                TextColor{ glm::vec3(1.0, 1.0, 1.0) };

		bool                     ShadowRenderCommand{};
		float					 ShadowOpacity{};
		glm::vec2                ShadowOffset{};
		glm::vec3                ShadowColor{};

		glm::vec3                RenderColor{};
		float                  RenderOpacity{};

		HDC                      hDC{};
		HFONT                    Font{};
		GLuint                   FontBase{};
		std::unordered_map <wchar_t, GLYPHMETRICSFLOAT> TextGlyph{};
		std::map <wchar_t, bool> GlyphCache{};
		std::vector<wchar_t>     TextVec{};
		int                      PrevSize{};

		int                      TextAlign{ ALIGN_DEFAULT };
		int                      RenderType{ RENDER_TYPE_STATIC };
		int                      HeightAlign{ HEIGHT_ALIGN_DEFAULT };
		bool                     FixMiddleCommand{};

	public:
		~Text();
		void Init(const wchar_t* FontName, int Type, int Italic = FALSE);
		void Reset(int RenderTypeFlag = RENDER_TYPE_STATIC);
		void SetColor(float R, float G, float B);
		void SetColor(glm::vec3& Color);
		void SetColorRGB(int R, int G, int B);
		void SetAlign(int AlignOpt);
		void SetLineGap(float Value);
		void EnableFixMiddle();
		void DisableFixMiddle();
		void SetHeightAlign(int Type);
		void EnableShadow();
		void DisableShadow();
		void SetShadow(float OffsetX, float OffsetY, float Opacity, glm::vec3 Color = glm::vec3(0.0, 0.0, 0.0));
		void Rotate(float RotationValue);
		void SetOpacity(float Value);
		void Render(glm::vec2& Position, float Size, const wchar_t* Fmt, ...);
		void Render(float X, float Y, float Size, const wchar_t* Fmt, ...);
		void RenderStr(glm::vec2& Position, float Size, std::string Str);
		void RenderStr(float X, float Y, float Size, std::string Str);
		void RenderWStr(glm::vec2& Position, float Size, std::wstring Str);
		void RenderWStr(float X, float Y, float Size, std::wstring Str);
		void SetRenderType(int Type);

	private:
		void InputText(std::vector<wchar_t>& Input, glm::vec2& Position, float Size);
		void ProcessText(wchar_t* Text, glm::vec2& Position, float Size);
		void GetLineLength(const wchar_t* Text);
		void CalculateTextLength(const wchar_t* Text);
		void NextLine();
		void TransformText();
		void PrepareRender();
		void ProcessGlyphCache(wchar_t* Text);
		bool CheckGlyphCache(wchar_t& Char);
		void LoadGlyph(wchar_t& Char);
	};
}