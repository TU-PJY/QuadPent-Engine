#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "SDKHeader.h"
#include <windows.h>
#include <map>

class TextUtil {
private:
	glm::vec2                RenderPosition{};
	GLfloat                  RenderStartPosition{};
	GLfloat                  Rotation{};

	size_t                   TextWordCount{};
	GLfloat                  TextLength{};
	GLfloat                  TextRenderSize{};
	GLfloat                  TextLineGap{};
	GLfloat                  MiddleHeight{};

	std::vector<GLfloat>     LineLengthBuffer{};
	std::wstring             CurrentText{};
	std::wstring             PrevText{};
	int                      CurrentLine{};

	GLfloat                  Opacity{ 1.0f };
	glm::vec3                TextColor{ glm::vec3(1.0, 1.0, 1.0) };

	HDC                      hDC{};
	HFONT                    Font{};
	GLuint                   FontBase{};
	GLYPHMETRICSFLOAT        TextGlyph[65536]{};
	std::map <wchar_t, bool> GlyphCache;

	int                      TextAlign{ ALIGN_DEFAULT };
	int                      RenderType{ RENDER_TYPE_STATIC };
	int                      HeightAlign{ HEIGHT_ALIGN_DEFAULT };
	bool                     FixMiddleCommand{};

public:
	~TextUtil();
	void Init(const wchar_t* FontName, int Type, int Italic=FALSE);
	void SetColor(GLfloat R, GLfloat G, GLfloat B);
	void SetColorRGB(int R, int G, int B);
	void SetAlign(int AlignOpt);
	void SetLineGap(GLfloat Value);
	void SetFixMiddle(bool Flag);
	void SetHeightAlign(int Type);
	void Rotate(GLfloat RotationValue);
	void SetOpacity(GLfloat Value);
	void SetRenderType(int Type);
	void RenderStr(GLfloat X, GLfloat Y, GLfloat Size, std::string& Str);
	void Render(GLfloat X, GLfloat Y, GLfloat Size, const wchar_t* Format, ...);

private:
	void GetLineLength(const wchar_t* Text);
	void CalculateTextLength(const wchar_t* Text);
	void NextLine();
	void TransformText();
	void PrepareRender();
	void ProcessGlyphCache(wchar_t* Text);
	bool CheckGlyphCache(wchar_t& Char);
	void LoadGlyph(wchar_t& Char);
};
extern TextUtil textUtil;