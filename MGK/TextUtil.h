#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "EngineHeader.h"
#include <windows.h>
#include <map>

enum Align { 
	ALIGN_DEFAULT, 
	ALIGN_MIDDLE, 
	ALIGN_LEFT 
};


class TextUtil {
private:
	glm::mat4 TranslateMatrix{ 1.0f }, RotateMatrix{ 1.0f }, ScaleMatrix{ 1.0f };

	glm::vec2 RenderPosition{};
	GLfloat Rotation{};

	size_t TextWordCount{};
	GLfloat TextLength{};
	GLfloat TextRenderSize{};
	GLfloat TextLineSpace{};
	GLfloat MiddleHeight{};

	std::vector<GLfloat> LineLength{};
	wchar_t* PrevText{};
	int CurrentLine{};

	GLfloat Transparency{ 1.0f };

	glm::vec3 TextColor{ glm::vec3(1.0, 1.0, 1.0) };
	std::map <wchar_t, bool> GlyphCache;

	HDC hDC{};
	HFONT Font{};
	GLuint FontBase{};
	GLYPHMETRICSFLOAT TextGlyph[65536]{};

	int TextAlign{ ALIGN_DEFAULT };
	int RenderType{ RENDER_TYPE_STATIC };
	bool ClampMiddleCommand{};


public:
	~TextUtil();
	void Init(const wchar_t* FontName, int Type, int Italic=FALSE);
	void SetColor(GLfloat R, GLfloat G, GLfloat B);
	void SetColorRGB(int R, int G, int B);
	void SetAlign(int AlignOpt);
	void SetLineSpace(GLfloat Value);
	void SetClampMiddle(bool Flag);
	void Rotate(GLfloat RotationValue);
	void SetRenderType(int Type);
	void Render(GLfloat X, GLfloat Y, GLfloat Size, GLfloat TransparencyValue, const wchar_t* Format, ...);

private:
	GLfloat GetLength(const wchar_t* Text);
	void GetLineLength(const wchar_t* Text);
	void CalculateTextLength(const wchar_t* Text);
	void SetNewLine();
	void TransformText();
	void PrepareRender();
	void ProcessGlyphCache(wchar_t* Text);
	bool CheckGlyphCache(wchar_t& Char);
	void LoadGlyph(wchar_t& Char);
};
extern TextUtil textUtil;