#include "TextUtil.h"
#include "CameraUtil.h"

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

void TextUtil::SetAlign(Align AlignOpt) {
	TextAlign = AlignOpt;
}

void TextUtil::Rotate(GLfloat Radians) {
	Rotation = Radians;
}

void TextUtil::SetLineSpace(GLfloat Value) {
	NewLineSpace = Value;
}

void TextUtil::LineNumber(int LineNum) {
	CurrentHeight -= GLfloat(LineNum) * NewLineSpace;
}

void TextUtil::ResetLineSpace() {
	NewLineSpace = 0.0;
}

void TextUtil::ResetLine() {
	CurrentHeight = 0.0;
}

void TextUtil::SetRenderType(RenderType Type) {
	if (Type == RenderType::Default)
		StaticRenderMode = false;
	else if (Type == RenderType::Static)
		StaticRenderMode = true;
}

void TextUtil::Render(GLfloat X, GLfloat Y, GLfloat Size, GLfloat TransparencyValue, const wchar_t* Format, ...) {
	wchar_t Text[256]{};

	va_list Args{};

	va_start(Args, Format);
	vswprintf(Text, sizeof(Text)/ sizeof(wchar_t), Format, Args);
	va_end(Args);

	unsigned char CharIndex{};
	GLfloat CurrentPositionX = 0.0f;
	GLfloat Length{};

	if (TextAlign != Align::Default)
		GetLength(Length, CharIndex, Text, Size);

	for (int i = 0; i < wcslen(Text); ++i) {
		InitMatrix();
		RotateMatrix = rotate(RotateMatrix, glm::radians(Rotation), glm::vec3(0.0, 0.0, 1.0));
		ScaleMatrix = scale(ScaleMatrix, glm::vec3(Size, Size, 0.0));

		switch (TextAlign) {
		case Align::Default:
			TranslateMatrix = translate(TranslateMatrix, glm::vec3(X + CurrentPositionX, Y + CurrentHeight, 0.0));
			break;
		case Align::Middle:
			TranslateMatrix = translate(TranslateMatrix, glm::vec3(X - Length / 2 + CurrentPositionX, Y + CurrentHeight, 0.0));
			break;
		case Align::Left:
			TranslateMatrix = translate(TranslateMatrix, glm::vec3(X - Length + CurrentPositionX, Y + CurrentHeight, 0.0));
			break;
		}

		Transparency = TransparencyValue;

		if(StaticRenderMode)
			camera.SetCamera(RenderType::Static);
		else
			camera.SetCamera(RenderType::Default);
		PrepareRender();

		if (Format == NULL)
			return;

		for (const auto& ch : Text) {
			if (!CheckGlyphCache(ch))
				LoadGlyph(ch);
		}

		glPushAttrib(GL_LIST_BIT);
		glListBase(FontBase);

		glCallLists(1, GL_UNSIGNED_SHORT, &Text[i]);
		glPopAttrib();

		unsigned int CharIndex = Text[i];
		if (CharIndex < 65536)
			CurrentPositionX += TextGlyph[CharIndex].gmfCellIncX * (Size / 1.0f);
	}
}



////////////////// private
void TextUtil::GetLength(GLfloat& Length, unsigned Index, const wchar_t* Text, GLfloat Size) {
	Length = 0.0f;
	for (int i = 0; i < wcslen(Text); ++i) {
		unsigned int CharIndex = Text[i];
		if (CharIndex < 65536)
			Length += TextGlyph[CharIndex].gmfCellIncX * (Size / 1.0f);
	}
}

void TextUtil::InitMatrix() {
	TranslateMatrix = glm::mat4(1.0f);
	RotateMatrix = glm::mat4(1.0f);
	ScaleMatrix = glm::mat4(1.0f);
}

void TextUtil::PrepareRender() {
	glUseProgram(TextShader);
	camera.PrepareRender(ShaderType::Text);

	TransparencyLocation = glGetUniformLocation(TextShader, "transparency");
	glUniform1f(TransparencyLocation, Transparency);

	ObjectColorLocation = glGetUniformLocation(TextShader, "objectColor");
	glUniform3f(ObjectColorLocation, TextColor.r, TextColor.g, TextColor.b);

	ModelLocation = glGetUniformLocation(TextShader, "model");
	glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, value_ptr(RotateMatrix * TranslateMatrix * ScaleMatrix));
}

bool TextUtil::CheckGlyphCache(wchar_t Char) {
	return GlyphCache.find(Char) != GlyphCache.end() && GlyphCache[Char];
}

void TextUtil::LoadGlyph(wchar_t Char) {
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