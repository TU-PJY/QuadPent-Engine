#include "SDK_Text.h"
#include "SDK_Camera.h"
#include "SDK_Transform.h"
#include "SDK_ComputeTool.h"
#include "SDK_StringTool.h"

glm::mat4 TextMatrix;

void MSDK::Text::Init(const wchar_t* FontName, int Type, int Italic) {
	hDC = wglGetCurrentDC();
	FontBase = glGenLists(65536);

	Font = CreateFont(
		-1, 0, 0, 0, Type, Italic, FALSE, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, CLIP_DEFAULT_PRECIS,
		NONANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, FontName
	);

	LineLengthBuffer.reserve(20);
}

void MSDK::Text::Reset(int RenderTypeFlag) {
	RenderType = RenderTypeFlag;
	TextAlign = ALIGN_DEFAULT;
	HeightAlign = HEIGHT_ALIGN_DEFAULT;
	FixMiddleCommand = false;
	ShadowRenderCommand = false;
	TextLineGap = 0.0;
	Rotation = 0.0;
	TextOpacity = 1.0;
}

void MSDK::Text::SetRenderType(int Type) {
	RenderType = Type;
}

void MSDK::Text::SetAlign(int AlignOpt) {
	TextAlign = AlignOpt;
}

void MSDK::Text::SetLineGap(float Value) {
	TextLineGap = Value;
}

void MSDK::Text::EnableFixMiddle() {
	FixMiddleCommand = true;
}

void MSDK::Text::DisableFixMiddle() {
	FixMiddleCommand = false;
}

void MSDK::Text::SetHeightAlign(int Type) {
	HeightAlign = Type;
}

void MSDK::Text::EnableShadow() {
	ShadowRenderCommand = true;
}

void MSDK::Text::DisableShadow() {
	ShadowRenderCommand = false;
}

void MSDK::Text::SetShadow(float OffsetX, float OffsetY, float Opacity, glm::vec3 Color) {
	ShadowOffset.x = OffsetX;
	ShadowOffset.y = OffsetY;
	ShadowOpacity = Opacity;
	ShadowColor = Color;
}

void MSDK::Text::SetColor(float R, float G, float B) {
	TextColor.r = R;
	TextColor.g = G;
	TextColor.b = B;
}

void MSDK::Text::SetColor(glm::vec3& Color) {
	TextColor.r = Color.r;
	TextColor.g = Color.g;
	TextColor.b = Color.b;
}

void MSDK::Text::SetColorRGB(int R, int G, int B) {
	TextColor.r = (1.0f / 255.0f) * (float)R;
	TextColor.g = (1.0f / 255.0f) * (float)G;
	TextColor.b = (1.0f / 255.0f) * (float)B;
}

void MSDK::Text::Rotate(float RotationValue) {
	Rotation = RotationValue;
}

void MSDK::Text::SetOpacity(float Value) {
	TextOpacity = Value;
}

void MSDK::Text::Render(glm::vec2& Position, float Size, const wchar_t* Fmt, ...) {
	if (Fmt == NULL)
		return;

	va_list Args{};
	va_start(Args, Fmt);

	TextVec.clear();

	int CurrentSize = vswprintf(nullptr, 0, Fmt, Args) + 1;
	if (PrevSize < CurrentSize) {
		TextVec.resize(CurrentSize);
		PrevSize = CurrentSize;
	}

	vswprintf(TextVec.data(), CurrentSize, Fmt, Args);

	va_end(Args);

	InputText(TextVec, Position, Size);
}

void MSDK::Text::Render(float X, float Y, float Size, const wchar_t* Fmt, ...) {
	if (Fmt == NULL)
		return;

	va_list Args{};
	va_start(Args, Fmt);

	TextVec.clear();

	int CurrentSize = vswprintf(nullptr, 0, Fmt, Args) + 1;
	if (PrevSize < CurrentSize) {
		TextVec.resize(CurrentSize);
		PrevSize = CurrentSize;
	}

	vswprintf(TextVec.data(), CurrentSize, Fmt, Args);

	va_end(Args);

	InputText(TextVec, glm::vec2(X, Y), Size);
}

void MSDK::Text::RenderStr(glm::vec2& Position, float Size, std::string Str) {
	Render(Position.x, Position.y, Size, MSDK::StringTool.Wstring(Str).c_str());
}

void MSDK::Text::RenderStr(float X, float Y, float Size, std::string Str) {
	Render(X, Y, Size, MSDK::StringTool.Wstring(Str).c_str());
}

void MSDK::Text::RenderWStr(glm::vec2& Position, float Size, std::wstring WStr) {
	Render(Position.x, Position.y, Size, WStr.c_str());
}

void MSDK::Text::RenderWStr(float X, float Y, float Size, std::wstring WStr) {
	Render(X, Y, Size, WStr.c_str());
}

////////////////// private
void MSDK::Text::InputText(std::vector<wchar_t>& Input, glm::vec2& Position, float Size) {
	CurrentText = std::wstring(Input.data());

	if (ShadowRenderCommand) {
		RenderColor = ShadowColor;
		RenderOpacity = TextOpacity * ShadowOpacity;
		ProcessText((wchar_t*)CurrentText.c_str(), glm::vec2(Position.x + ShadowOffset.x * Size, Position.y + ShadowOffset.y * Size), Size);
	}

	RenderColor = TextColor;
	RenderOpacity = TextOpacity;
	ProcessText((wchar_t*)CurrentText.c_str(), Position, Size);
}

void MSDK::Text::ProcessText(wchar_t* Text, glm::vec2& Position, float Size) {
	CurrentLine = 0;
	TextRenderSize = Size;
	RenderPosition = Position;
	CurrentRenderOffset = glm::vec2(0.0, 0.0);

	if (CurrentText != PrevText) {
		TextWordCount = wcslen(Text);
		ProcessGlyphCache(Text);
		PrevText = CurrentText;
	}

	CalculateTextLength(Text);

	switch (HeightAlign) {
	case HEIGHT_ALIGN_MIDDLE:
		RenderPosition.y -= TextRenderSize * 0.5;
		break;

	case HEIGHT_ALIGN_UNDER:
		RenderPosition.y -= TextRenderSize;
		break;
	}

	for (int i = 0; i < TextWordCount; ++i) {
		if (Text[i] == L'\n') {
			NextLine();
			continue;
		}

		TransformText();
		Camera.SetCamera(RenderType);
		PrepareRender();

		glPushAttrib(GL_LIST_BIT);
		glListBase(FontBase);
		glCallLists(1, GL_UNSIGNED_SHORT, &Text[i]);
		glPopAttrib();

		unsigned int CharIndex = Text[i];
		if (CharIndex < 65536)
			CurrentRenderOffset.x += TextGlyph[CharIndex].gmfCellIncX * (TextRenderSize / 1.0f);
	}
}

void MSDK::Text::GetLineLength(const wchar_t* Text) {
	LineLengthBuffer.clear();
	float CurrentLineLength{};

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

void MSDK::Text::CalculateTextLength(const wchar_t* Text) {
	GetLineLength(Text);
	TextLength = LineLengthBuffer[0];

	MiddleHeight = 0.0;
	if (FixMiddleCommand) {
		size_t LineNum = LineLengthBuffer.size();

		if (LineNum > 1) {
			for (int i = 0; i < LineNum; ++i)
				MiddleHeight += (TextLineGap + TextRenderSize);
			MiddleHeight /= 2.0;
		}
	}
}

void MSDK::Text::NextLine() {
	CurrentRenderOffset.x = 0.0;
	CurrentRenderOffset.y -= (TextLineGap + TextRenderSize);

	if (TextAlign != ALIGN_DEFAULT) {
		++CurrentLine;
		TextLength = LineLengthBuffer[CurrentLine];
	}
}

void MSDK::Text::TransformText() {
	MSDK::Transform.Identity(TextMatrix);
	MSDK::Transform.Move(TextMatrix, RenderPosition.x, RenderPosition.y + MiddleHeight);

	switch (TextAlign) {
	case ALIGN_DEFAULT:
		MSDK::Transform.Rotate(TextMatrix, Rotation);
		MSDK::Transform.Move(TextMatrix, CurrentRenderOffset);
		break;

	case ALIGN_MIDDLE:
		MSDK::Transform.Rotate(TextMatrix, Rotation);
		MSDK::Transform.Move(TextMatrix, -TextLength * 0.5, 0.0);
		MSDK::Transform.Move(TextMatrix, CurrentRenderOffset);
		break;

	case ALIGN_LEFT:
		MSDK::Transform.Rotate(TextMatrix, Rotation);
		MSDK::Transform.Move(TextMatrix, -TextLength, 0.0);
		MSDK::Transform.Move(TextMatrix, CurrentRenderOffset);
		break;
	}

	MSDK::Transform.Scale(TextMatrix, TextRenderSize, TextRenderSize);
}

void MSDK::Text::ProcessGlyphCache(wchar_t* Text) {
	for (int i = 0; i < TextWordCount; ++i) {
		if (!CheckGlyphCache(Text[i]))
			LoadGlyph(Text[i]);
	}
}

bool MSDK::Text::CheckGlyphCache(wchar_t& Char) {
	return GlyphCache.find(Char) != GlyphCache.end() && GlyphCache[Char];
}

void MSDK::Text::LoadGlyph(wchar_t& Char) {
	if (Char >= 65536)
		return;

	HFONT OldFont = (HFONT)SelectObject(hDC, Font);
	GLYPHMETRICSFLOAT Glyph;
	wglUseFontOutlinesW(hDC, Char, 1, FontBase + Char, 0.0f, 0.0f, WGL_FONT_POLYGONS, &Glyph);
	TextGlyph[Char] = Glyph;
	SelectObject(hDC, OldFont);
	GlyphCache[Char] = true;
}

void MSDK::Text::PrepareRender() {
	glUseProgram(TEXT_SHADER);
	Camera.PrepareRender(SHADER_TYPE_TEXT);

	glUniform1f(TEXT_OPACITY_LOCATION, RenderOpacity);
	glUniform3f(TEXT_COLOR_LOCATION, RenderColor.r, RenderColor.g, RenderColor.b);
	glUniformMatrix4fv(TEXT_MODEL_LOCATION, 1, GL_FALSE, value_ptr(TextMatrix));
}

MSDK::Text::~Text() {
	HFONT OldFont = (HFONT)SelectObject(hDC, Font);
	SelectObject(hDC, OldFont);
	DeleteObject(Font);
	glDeleteLists(FontBase, 65536);
	DeleteDC(hDC);
}