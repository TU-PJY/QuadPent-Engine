#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "MATA_HEADER.h"
#include <windows.h>

enum class Align
{ Default, Middle, Left };

enum class TextRenderMode
{ Static, Default };

// for unicode rendering
class TextUtilUnicode {
private:
	glm::mat4 TranslateMatrix{ 1.0f }, RotateMatrix{ 1.0f }, ScaleMatrix{ 1.0f };
	GLfloat Rotation{};
	GLfloat Transparency{ 1.0f };
	glm::vec3 TextColor{ glm::vec3(1.0, 1.0, 1.0) };
	Align TextAlign{ Align::Default };

	unsigned int ModelLocation{}, TransparencyLocation{}, ObjectColorLocation{};

	HDC hDC{};
	GLuint FontBase{};
	GLYPHMETRICSFLOAT TextGlyph[65536]{};

public:
	~TextUtilUnicode();
	void Init(const wchar_t* FontName, int Type, int Italic=FALSE);
	void SetColor(GLfloat R, GLfloat G, GLfloat B);
	void SetAlign(Align AlignOpt);
	void Rotate(GLfloat Radians);
	void Render(TextRenderMode Mode, GLfloat X, GLfloat Y, GLfloat Size, GLfloat TransparencyValue, const wchar_t* Format, ...);
	void GetLength(GLfloat& Length, unsigned Index, const wchar_t* Text, GLfloat Size);
	void InitTransform();
	void ProcessTransform();
};
extern TextUtilUnicode textUnicode;

// for rendering ansi rendering
class TextUtil {
private:
	glm::mat4 TranslateMatrix{ 1.0f }, RotateMatrix{ 1.0f }, ScaleMatrix{ 1.0f };
	GLfloat Rotation{};
	GLfloat Transparency{ 1.0f };
	glm::vec3 TextColor{ glm::vec3(1.0, 1.0, 1.0) };
	Align TextAlign{ Align::Default };

	unsigned int ModelLocation{}, TransparencyLocation{}, ObjectColorLocation{};

	HDC hDC{};
	GLuint FontBase{};
	GLYPHMETRICSFLOAT TextGlyph[96]{};

public:
	~TextUtil();
	void Init(const wchar_t* FontName, int Type, int Italic=FALSE);
	void SetColor(GLfloat R, GLfloat G, GLfloat B);
	void SetAlign(Align AlignOpt);
	void Rotate(GLfloat Radians);
	void Render(TextRenderMode Mode, GLfloat X, GLfloat Y, GLfloat Size, GLfloat TransparencyValue, const char* Format, ...);
	void GetLength(GLfloat& Length, unsigned Index, const char* Text, GLfloat Size);
	void InitTransform();
	void ProcessTransform();
};