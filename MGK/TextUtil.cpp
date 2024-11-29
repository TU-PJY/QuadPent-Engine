#include "TextUtil.h"
#include "CameraUtil.h"
#include "TransformUtil.h"
#include "ComputeUtil.h"

void TextUtil::Init(const wchar_t* FontName, int Type, int Italic) {
	hDC = wglGetCurrentDC();
	FontBase = glGenLists(65536);

	Font = CreateFont(
		-1, 0, 0, 0, Type, Italic, FALSE, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, CLIP_DEFAULT_PRECIS,
		NONANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, FontName
	);

	LineLengthBuffer.reserve(20);
}

void TextUtil::SetRenderType(int Type) {
	RenderType = Type;
}

void TextUtil::SetAlign(int AlignOpt) {
	TextAlign = AlignOpt;
}

void TextUtil::SetLineSpace(GLfloat Value) {
	TextLineSpace = Value;
}

void TextUtil::SetClampMiddle(bool Flag) {
	ClampMiddleCommand = Flag;
}

void TextUtil::SetHeightMiddle(bool Flag) {
	HeightMiddleCommand = Flag;
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

void TextUtil::Rotate(GLfloat RotationValue) {
	Rotation = RotationValue;
}

void TextUtil::SetAlpha(GLfloat Value) {
	TextAlphaValue = Value;
}

void TextUtil::AddString(std::string& Str, std::string& AddStr) {
	Str += AddStr;
}

void TextUtil::EraseString(std::string& Str) {
	Str.pop_back();
}

void TextUtil::RemoveString(std::string& Str, std::string& RemoveStr) {
	size_t Pos = Str.find(RemoveStr);
	while (Pos != std::string::npos) {
		Str.erase(Pos, RemoveStr.length());
		Pos = Str.find(RemoveStr);
	}
}

std::wstring TextUtil::W(const std::string& Str) {
	int SizeNeed = MultiByteToWideChar(CP_UTF8, 0, &Str[0], (int)Str.size(), NULL, 0);
	std::wstring Wstr(SizeNeed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &Str[0], (int)Str.size(), &Wstr[0], SizeNeed);

	return Wstr;
}

void TextUtil::RenderStr(GLfloat X, GLfloat Y, GLfloat Size, std::string& Str) {
	Render(X, Y, Size, W(Str).c_str());
}

void TextUtil::Render(GLfloat X, GLfloat Y, GLfloat Size, const wchar_t* Format, ...) {
	wchar_t Text[256]{};
	va_list Args{};

	va_start(Args, Format);
	vswprintf(Text, sizeof(Text)/ sizeof(wchar_t), Format, Args);
	va_end(Args);

	if (Format == NULL)  
		return;

	TextWordCount = wcslen(Text);
	ProcessGlyphCache(Text);
	CalculateTextLength(Text);

	TextRenderSize = Size;
	RenderPosition = glm::vec2(X, Y);
	if (HeightMiddleCommand)
		RenderPosition.y -= TextRenderSize * 0.5;

	Transform::Identity(RotateMatrix);
	Transform::Identity(ScaleMatrix);
	Transform::Rotate(RotateMatrix, Rotation);
	Transform::Scale(ScaleMatrix, TextRenderSize, TextRenderSize);

	for (int i = 0; i < TextWordCount; ++i) {
		if (Text[i] == L'\n') {
			SetNewLine();
			continue;
		}

		TransformText();
		camera.SetCamera(RenderType);
		PrepareRender();

		glPushAttrib(GL_LIST_BIT);
		glListBase(FontBase);
		glCallLists(1, GL_UNSIGNED_SHORT, &Text[i]);
		glPopAttrib();

		unsigned int CharIndex = Text[i];
		if (CharIndex < 65536)
			RenderPosition.x += TextGlyph[CharIndex].gmfCellIncX * (TextRenderSize / 1.0f);
	}
}


////////////////// private
GLfloat TextUtil::GetLength(const wchar_t* Text) {
	GLfloat Length{};
	for (int i = 0; i < wcslen(Text); ++i) {
		unsigned int CharIndex = Text[i];
		if (CharIndex < 65536)
			Length += TextGlyph[CharIndex].gmfCellIncX * (TextRenderSize / 1.0f);
	}

	return Length;
}

void TextUtil::GetLineLength(const wchar_t* Text) {
	LineLengthBuffer.clear();
	GLfloat CurrentLineLength{};

	for (int i = 0; i < wcslen(Text); ++i) {
		if (Text[i] == L'\n') {
			LineLengthBuffer.emplace_back(CurrentLineLength);
			CurrentLineLength = 0.0f; 
			continue;
		}

		unsigned int CharIndex = Text[i];
		if (CharIndex < 65536) 
			CurrentLineLength += TextGlyph[CharIndex].gmfCellIncX * (TextRenderSize / 1.0f);
	}

	if (CurrentLineLength > 0.0f)
		LineLengthBuffer.emplace_back(CurrentLineLength);
}

void TextUtil::CalculateTextLength(const wchar_t* Text) {
	switch (TextAlign) {
	case ALIGN_MIDDLE: case ALIGN_LEFT:
		GetLineLength(Text);
		TextLength = LineLengthBuffer[0];

		MiddleHeight = 0.0;
		if (ClampMiddleCommand) {
			size_t LineNum = LineLengthBuffer.size();
			for (int i = 0; i < LineNum; ++i)
				MiddleHeight += TextLineSpace;
			MiddleHeight /= 2.0;
		}
		break;

	case ALIGN_DEFAULT:
		TextLength = GetLength(Text);
		break;
	}

	CurrentLine = 0;
}

void TextUtil::SetNewLine() {
	RenderPosition.y -= TextLineSpace;
	RenderPosition.x = 0.0;

	if (TextAlign != ALIGN_DEFAULT) {
		++CurrentLine;
		TextLength = LineLengthBuffer[CurrentLine];
	}
}

void TextUtil::TransformText() {
	Transform::Identity(TranslateMatrix);

	switch (TextAlign) {
	case ALIGN_DEFAULT:
		Transform::Move(TranslateMatrix, RenderPosition.x, RenderPosition.y + MiddleHeight);
		break;

	case ALIGN_MIDDLE:
		Transform::Move(TranslateMatrix, RenderPosition.x - (TextLength / 2.0), RenderPosition.y + MiddleHeight);
		break;

	case ALIGN_LEFT:
		Transform::Move(TranslateMatrix, RenderPosition.x - TextLength, RenderPosition.y + MiddleHeight);
		break;
	}
}

void TextUtil::PrepareRender() {
	Compt::ComputeMatrix(ResultMatrix, RotateMatrix, TranslateMatrix, ScaleMatrix);

	glUseProgram(TEXT_SHADER);
	camera.PrepareRender(SHADER_TYPE_TEXT);

	glUniform1f(TEXT_ALPHA_LOCATION, TextAlphaValue);
	glUniform3f(TEXT_COLOR_LOCATION, TextColor.r, TextColor.g, TextColor.b);
	glUniformMatrix4fv(TEXT_MODEL_LOCATION, 1, GL_FALSE, value_ptr(ResultMatrix));
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