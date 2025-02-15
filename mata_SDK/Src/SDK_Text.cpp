#include "SDK_Text.h"
#include "SDK_Camera.h"
#include "SDK_Transform.h"
#include "SDK_ComputeTool.h"
#include "SDK_StringTool.h"

void SDK::Text::Init(const wchar_t* FontName, int Type, int Italic) {
	hDC = wglGetCurrentDC();
	FontBase = glGenLists(65536);

	Font = CreateFont(
		-1, 0, 0, 0, Type, Italic, FALSE, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, CLIP_DEFAULT_PRECIS,
		NONANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, FontName
	);

	LineLengthBuffer.reserve(20);
}

void SDK::Text::Reset(int RenderTypeFlag) {
	RenderType = RenderTypeFlag;
	TextAlign = ALIGN_DEFAULT;
	HeightAlign = HEIGHT_ALIGN_DEFAULT;
	FixMiddleCommand = false;
	ShadowRenderCommand = false;
	TextLineGap = 0.0;
	Rotation = 0.0;
	TextOpacity = 1.0;
}

void SDK::Text::SetRenderType(int Type) {
	RenderType = Type;
}

void SDK::Text::SetAlign(int AlignOpt) {
	TextAlign = AlignOpt;
}

void SDK::Text::SetLineGap(GLfloat Value) {
	TextLineGap = Value;
}

void SDK::Text::EnableFixMiddle() {
	FixMiddleCommand = true;
}

void SDK::Text::DisableFixMiddle() {
	FixMiddleCommand = false;
}

void SDK::Text::SetHeightAlign(int Type) {
	HeightAlign = Type;
}

void SDK::Text::EnableShadow() {
	ShadowRenderCommand = true;
}

void SDK::Text::DisableShadow() {
	ShadowRenderCommand = false;
}

void SDK::Text::SetShadow(GLfloat OffsetX, GLfloat OffsetY, GLfloat Opacity, glm::vec3 Color) {
	ShadowOffset.x = OffsetX;
	ShadowOffset.y = OffsetY;
	ShadowOpacity = Opacity;
	ShadowColor = Color;
}

void SDK::Text::SetColor(GLfloat R, GLfloat G, GLfloat B) {
	TextColor.r = R;
	TextColor.g = G;
	TextColor.b = B;
}

void SDK::Text::SetColor(glm::vec3& Color) {
	TextColor.r = Color.r;
	TextColor.g = Color.g;
	TextColor.b = Color.b;
}

void SDK::Text::SetColorRGB(int R, int G, int B) {
	TextColor.r = (1.0f / 255.0f) * (GLfloat)R;
	TextColor.g = (1.0f / 255.0f) * (GLfloat)G;
	TextColor.b = (1.0f / 255.0f) * (GLfloat)B;
}

void SDK::Text::Rotate(GLfloat RotationValue) {
	Rotation = RotationValue;
}

void SDK::Text::SetOpacity(GLfloat Value) {
	TextOpacity = Value;
}

void SDK::Text::Render(glm::vec2& Position, GLfloat Size, const wchar_t* Fmt, ...) {
	if (Fmt == NULL)
		return;

	va_list Args{};
	va_start(Args, Fmt);
	std::vector<wchar_t> Text(vswprintf(nullptr, 0, Fmt, Args) + 1);
	va_end(Args);

	va_start(Args, Fmt);
	vswprintf(Text.data(), Text.size(), Fmt, Args);
	va_end(Args);

	InputText(Text, Position, Size);
}

void SDK::Text::Render(GLfloat X, GLfloat Y, GLfloat Size, const wchar_t* Fmt, ...) {
	if (Fmt == NULL)
		return;

	va_list Args{};
	va_start(Args, Fmt);
	std::vector<wchar_t> Text(vswprintf(nullptr, 0, Fmt, Args) + 1);
	va_end(Args);

	va_start(Args, Fmt);
	vswprintf(Text.data(), Text.size(), Fmt, Args);
	va_end(Args);

	InputText(Text, glm::vec2(X, Y), Size);
}

void SDK::Text::RenderStr(glm::vec2& Position, GLfloat Size, std::string Str) {
	Render(Position.x, Position.y, Size, SDK::StringTool.Wstring(Str).c_str());
}

void SDK::Text::RenderStr(GLfloat X, GLfloat Y, GLfloat Size, std::string Str) {
	Render(X, Y, Size, SDK::StringTool.Wstring(Str).c_str());
}

void SDK::Text::RenderWStr(glm::vec2& Position, GLfloat Size, std::wstring WStr) {
	Render(Position.x, Position.y, Size, WStr.c_str());
}

void SDK::Text::RenderWStr(GLfloat X, GLfloat Y, GLfloat Size, std::wstring WStr) {
	Render(X, Y, Size, WStr.c_str());
}

////////////////// private
void SDK::Text::InputText(std::vector<wchar_t>& Input, glm::vec2& Position, GLfloat Size) {
	CurrentText = std::wstring(Input.data());

	if (ShadowRenderCommand) {
		RenderColor = ShadowColor;
		RenderOpacity = TextOpacity * ShadowOpacity;
		ProcessText(Input.data(), Position + ShadowOffset, Size);
	}

	RenderColor = TextColor;
	RenderOpacity = TextOpacity;
	ProcessText(Input.data(), Position, Size);
}

void SDK::Text::ProcessText(wchar_t* Text, glm::vec2 Position, GLfloat Size) {
	CurrentLine = 0;
	TextRenderSize = Size;
	RenderPosition = Position;
	CurrentRenderPosition = 0.0;

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
			CurrentRenderPosition += TextGlyph[CharIndex].gmfCellIncX * (TextRenderSize / 1.0f);
	}
}

void SDK::Text::GetLineLength(const wchar_t* Text) {
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

void SDK::Text::CalculateTextLength(const wchar_t* Text) {
	GetLineLength(Text);
	TextLength = LineLengthBuffer[0];

	MiddleHeight = 0.0;
	if (FixMiddleCommand) {
		size_t LineNum = LineLengthBuffer.size();
		for (int i = 0; i < LineNum; ++i)
			MiddleHeight += (TextLineGap + TextRenderSize);
		MiddleHeight /= 2.0;
	}
}

void SDK::Text::NextLine() {
	RenderPosition.y -= (TextLineGap + TextRenderSize);
	CurrentRenderPosition = 0.0;

	if (TextAlign != ALIGN_DEFAULT) {
		++CurrentLine;
		TextLength = LineLengthBuffer[CurrentLine];
	}
}

void SDK::Text::TransformText() {
	SDK::Transform.Identity(TextMatrix);
	SDK::Transform.Move(TextMatrix, RenderPosition.x, RenderPosition.y + MiddleHeight);

	switch (TextAlign) {
	case ALIGN_DEFAULT:
		SDK::Transform.Rotate(TextMatrix, Rotation);
		SDK::Transform.Move(TextMatrix, CurrentRenderPosition, 0.0);
		break;

	case ALIGN_MIDDLE:
		SDK::Transform.Move(TextMatrix, -TextLength / 2.0, 0.0);
		SDK::Transform.Rotate(TextMatrix, Rotation);
		SDK::Transform.Move(TextMatrix, CurrentRenderPosition, 0.0);
		break;

	case ALIGN_LEFT:
		SDK::Transform.Move(TextMatrix, -TextLength, 0.0);
		SDK::Transform.Rotate(TextMatrix, Rotation);
		SDK::Transform.Move(TextMatrix, CurrentRenderPosition, 0.0);
		break;
	}

	SDK::Transform.Scale(TextMatrix, TextRenderSize, TextRenderSize);
}

void SDK::Text::ProcessGlyphCache(wchar_t* Text) {
	for (int i = 0; i < TextWordCount; ++i) {
		if (!CheckGlyphCache(Text[i]))
			LoadGlyph(Text[i]);
	}
}

bool SDK::Text::CheckGlyphCache(wchar_t& Char) {
	return GlyphCache.find(Char) != GlyphCache.end() && GlyphCache[Char];
}

void SDK::Text::LoadGlyph(wchar_t& Char) {
	if (Char >= 65536)
		return;

	HFONT OldFont = (HFONT)SelectObject(hDC, Font);
	GLYPHMETRICSFLOAT Glyph;
	wglUseFontOutlinesW(hDC, Char, 1, FontBase + Char, 0.0f, 0.0f, WGL_FONT_POLYGONS, &Glyph);
	TextGlyph[Char] = Glyph;
	SelectObject(hDC, OldFont);
	GlyphCache[Char] = true;
}

void SDK::Text::PrepareRender() {
	glUseProgram(TEXT_SHADER);
	Camera.PrepareRender(SHADER_TYPE_TEXT);

	glUniform1f(TEXT_OPACITY_LOCATION, RenderOpacity);
	glUniform3f(TEXT_COLOR_LOCATION, RenderColor.r, RenderColor.g, RenderColor.b);
	glUniformMatrix4fv(TEXT_MODEL_LOCATION, 1, GL_FALSE, value_ptr(TextMatrix));
}

SDK::Text::~Text() {
	HFONT OldFont = (HFONT)SelectObject(hDC, Font);
	SelectObject(hDC, OldFont);
	DeleteObject(Font);
	glDeleteLists(FontBase, 65536);
	DeleteDC(hDC);
}