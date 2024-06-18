#pragma once
#include "RenderModeUtil.h"
#include <windows.h>

enum class Align
{ Default, Middle, Left };

class Text {
private:
	glm::mat4 TranslateMatrix{ 1.0f }, RotateMatrix{ 1.0f }, ScaleMatrix{ 1.0f };
	GLfloat Rotation{};
	GLfloat AlphaValue{ 1.0f };
	glm::vec3 TextColor{ glm::vec3(1.0, 1.0, 1.0) };
	Align TextAlign{ Align::Default };

	unsigned int ModelLocation{}, TransparencyLocation{}, ObjectColorLocation{};

	HDC hDC{};
	GLuint FontBase{};
	GLYPHMETRICSFLOAT TextGlyph[96]{};

public:
	~Text() {
		glDeleteLists(FontBase, 96);
		DeleteDC(hDC);
	}

	void Init(const char* FontName, int type) {
		hDC = wglGetCurrentDC();
		BuildFont(FontName, type);
	}

	void BuildFont(const char* FontName, int type) {
		HFONT Font;
		HFONT OldFont;

		FontBase = glGenLists(96);

		Font = CreateFont(
			-1, 0, 0, 0, type, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
			ANTIALIASED_QUALITY, FF_DONTCARE | VARIABLE_PITCH, FontName
		);

		OldFont = (HFONT)SelectObject(hDC, Font);
		wglUseFontOutlines(hDC, 32, 96, FontBase, 0.0f, 0.0f, WGL_FONT_POLYGONS, TextGlyph);

		SelectObject(hDC, OldFont);
		DeleteObject(Font);
	}

	void SetAlpha(GLfloat Value) {
		AlphaValue = Value;
	}

	void SetColor(GLfloat R, GLfloat G, GLfloat B) {
		TextColor.r = R;
		TextColor.g = G;
		TextColor.b = B;
	}

	void SetAlign(Align AlignOpt) {
		TextAlign = AlignOpt;
	}

	void Rotate(GLfloat Radians) {
		Rotation = Radians;
	}

	void Draw(GLfloat X, GLfloat Y, GLfloat Size, const char* Format, ...) {
		char Text[96];
		va_list Args{};
		va_start(Args, Format);
		vsprintf(Text, Format, Args);
		va_end(Args);

		unsigned char CharIndex{};
		GLfloat CurrentPositionX = 0.0f;
		GLfloat Length{};

		if (TextAlign != Align::Default)
			GetLength(Length, CharIndex, Text, Size);

		for (int i = 0; i < strlen(Text); ++i) {
			InitTransform();
			RotateMatrix = rotate(RotateMatrix, glm::radians(Rotation), glm::vec3(0.0, 0.0, 1.0));
			ScaleMatrix = scale(ScaleMatrix, glm::vec3(Size, Size, 0.0));

			switch (TextAlign) {
			case Align::Default:
				TranslateMatrix = translate(TranslateMatrix, glm::vec3(X + CurrentPositionX, Y, 0.0));
				break;

			case Align::Middle:
				TranslateMatrix = translate(TranslateMatrix, glm::vec3(X - Length / 2 + CurrentPositionX, Y, 0.0));
				break;

			case Align::Left:
				TranslateMatrix = translate(TranslateMatrix, glm::vec3(X - Length + CurrentPositionX, Y, 0.0));
				break;
			}

			ProcessTransform();

			if (Format == NULL)
				return;

			glPushAttrib(GL_LIST_BIT);
			glListBase(FontBase - 32);
			glCallLists(1, GL_UNSIGNED_BYTE, &Text[i]);
			glPopAttrib();

			CharIndex = Text[i] - 32;
			if (CharIndex < sizeof(TextGlyph) / sizeof(TextGlyph[i]))
				CurrentPositionX += TextGlyph[CharIndex].gmfCellIncX * (Size / 1.0);
		}
	}

	void GetLength(GLfloat& Length, unsigned Index, char* Text, GLfloat Size) {
		for (int i = 0; i < strlen(Text); ++i) {
			Index = Text[i] - 32;
			if (Index < sizeof(TextGlyph) / sizeof(TextGlyph[i]))
				Length += TextGlyph[Index].gmfCellIncX * (Size / 1.0);
		}
	}

	void InitTransform() {
		TranslateMatrix = glm::mat4(1.0f);
		RotateMatrix = glm::mat4(1.0f);
		ScaleMatrix = glm::mat4(1.0f);
	}

	void ProcessTransform() {
		renderMode.SetTextMode();

		TransparencyLocation = glGetUniformLocation(TextShader, "transparency");
		glUniform1f(TransparencyLocation, AlphaValue);

		ModelLocation = glGetUniformLocation(TextShader, "model");
		glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, value_ptr(RotateMatrix * TranslateMatrix * ScaleMatrix));

		ObjectColorLocation = glGetUniformLocation(TextShader, "objectColor");
		glUniform3f(ObjectColorLocation, TextColor.r, TextColor.g, TextColor.b);

		ModelLocation = glGetUniformLocation(TextShader, "model");
		glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, value_ptr(RotateMatrix * TranslateMatrix * ScaleMatrix));
	}
};