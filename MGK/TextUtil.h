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
	GLfloat Offset{};
	GLfloat Rotation{};

	size_t TextWordCount{};
	GLfloat TextLength{};
	GLfloat TextRenderSize{};

	GLfloat Transparency{ 1.0f };

	glm::vec3 TextColor{ glm::vec3(1.0, 1.0, 1.0) };
	std::map <wchar_t, bool> GlyphCache;

	HDC hDC{};
	HFONT Font{};
	GLuint FontBase{};
	GLYPHMETRICSFLOAT TextGlyph[65536]{};

	int TextAlign{ ALIGN_DEFAULT };
	int RenderType{ RENDER_TYPE_STATIC };


public:
	~TextUtil();
	void Init(const wchar_t* FontName, int Type, int Italic=FALSE);
	void SetColor(GLfloat R, GLfloat G, GLfloat B);
	void SetColorRGB(int R, int G, int B);
	void SetAlign(int AlignOpt);
	void Rotate(GLfloat RotationValue);
	void SetRenderType(int Type);
	void Render(GLfloat X, GLfloat Y, GLfloat Size, GLfloat TransparencyValue, const wchar_t* Format, ...);

private:
	GLfloat GetLength(unsigned Index, const wchar_t* Text);
	void PrepareRender();
	void ProcessGlyphCache(wchar_t* Text);
	bool CheckGlyphCache(wchar_t& Char);
	void LoadGlyph(wchar_t& Char);
};
extern TextUtil textUtil;