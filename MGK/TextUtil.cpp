#include "TextUtil.h"
#include "CameraUtil.h"
#include "TransformUtil.h"

void TextUtil::Init(const wchar_t* FontName, int Type, int Italic) {
	hDC = wglGetCurrentDC();
	FontBase = glGenLists(65536);

	Font = CreateFont(
		-1, 0, 0, 0, Type, Italic, FALSE, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, CLIP_DEFAULT_PRECIS,
		NONANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, FontName
	);
}

void TextUtil::SetColor(GLfloat R, GLfloat G, GLfloat B) {
	TextColor.r = R;
	TextColor.g = G;
	TextColor.b = B;
}

void TextUtil::SetColorRGB(int R, int G, int B) {
	TextColor.r = (1.0f / 255.0f) * (GLfloat)R;
	TextColor.g = (1.0f / 255.0f) * (GLfloat)G;
	TextColor.b = (1.0f / 255.0f) * (GLfloat)B;
}

void TextUtil::SetAlign(int AlignOpt) {
	TextAlign = AlignOpt;
}

void TextUtil::Rotate(GLfloat RotationValue) {
	Rotation = RotationValue;
}

void TextUtil::SetRenderType(int Type) {
	RenderType = Type;
}

void TextUtil::Render(GLfloat X, GLfloat Y, GLfloat Size, GLfloat TransparencyValue, const wchar_t* Format, ...) {
	wchar_t Text[256]{};
	va_list Args{};

	va_start(Args, Format);
	vswprintf(Text, sizeof(Text)/ sizeof(wchar_t), Format, Args);
	va_end(Args);

	if (Format == NULL)  
		return;

	unsigned char CharIndex{};

	TextWordCount = wcslen(Text);
	ProcessGlyphCache(Text);

	if (TextAlign != ALIGN_DEFAULT)
		TextLength = GetLength(CharIndex, Text);

	Offset = 0.0;
	RenderPosition = glm::vec2(X, Y);
	TextRenderSize = Size;
	Transparency = TransparencyValue;

	Transform::Identity(RotateMatrix);
	Transform::Identity(ScaleMatrix);
	Transform::Rotate(RotateMatrix, Rotation);
	Transform::Scale(ScaleMatrix, TextRenderSize, TextRenderSize);

	for (int i = 0; i < TextWordCount; ++i) {
		Transform::Identity(TranslateMatrix);

		switch (TextAlign) {
		case ALIGN_DEFAULT:
			Transform::Move(TranslateMatrix, RenderPosition.x + Offset, RenderPosition.y);
			break;

		case ALIGN_MIDDLE:
			Transform::Move(TranslateMatrix, RenderPosition.x - (TextLength / 2.0) + Offset, RenderPosition.y);
			break;

		case ALIGN_LEFT:
			Transform::Move(TranslateMatrix, RenderPosition.x - TextLength + Offset, RenderPosition.y);
			break;
		}
		camera.SetCamera(RenderType);
		PrepareRender();

		glPushAttrib(GL_LIST_BIT);
		glListBase(FontBase);

		glCallLists(1, GL_UNSIGNED_SHORT, &Text[i]);
		glPopAttrib();

		unsigned int CharIndex = Text[i];
		if (CharIndex < 65536)
			Offset += TextGlyph[CharIndex].gmfCellIncX * (TextRenderSize / 1.0f);
	}
}


////////////////// private
GLfloat TextUtil::GetLength(unsigned Index, const wchar_t* Text) {
	GLfloat Length{};
	for (int i = 0; i < wcslen(Text); ++i) {
		unsigned int CharIndex = Text[i];
		if (CharIndex < 65536)
			Length += TextGlyph[CharIndex].gmfCellIncX * (TextRenderSize / 1.0f);
	}

	return Length;
}

void TextUtil::PrepareRender() {
	glUseProgram(TextShader);
	camera.PrepareRender(SHADER_TYPE_TEXT);

	glUniform1f(TextTransparencyLocation, Transparency);
	glUniform3f(TextColorLocation, TextColor.r, TextColor.g, TextColor.b);
	glUniformMatrix4fv(TextModelLocation, 1, GL_FALSE, value_ptr(RotateMatrix * TranslateMatrix * ScaleMatrix));
}

void TextUtil::ProcessGlyphCache(wchar_t* Text) {
	for (int i = 0; i < TextWordCount; ++i) {
		if (!CheckGlyphCache(Text[i]))
			LoadGlyph(Text[i]);
	}
}

bool TextUtil::CheckGlyphCache(wchar_t& Char) {
	return GlyphCache.find(Char) != GlyphCache.end() && GlyphCache[Char];
}

void TextUtil::LoadGlyph(wchar_t& Char) {
	if (Char >= 65536)
		return;

	HFONT OldFont = (HFONT)SelectObject(hDC, Font);
	wglUseFontOutlinesW(hDC, Char, 1, FontBase + Char, 0.0f, 0.0f, WGL_FONT_POLYGONS, &TextGlyph[Char]);
	SelectObject(hDC, OldFont);
	GlyphCache[Char] = true;
}

TextUtil::~TextUtil() {
	HFONT OldFont = (HFONT)SelectObject(hDC, Font);
	SelectObject(hDC, OldFont);
	DeleteObject(Font);
	glDeleteLists(FontBase, 65536);
	DeleteDC(hDC);
}