#include "SDK_Text.h"
#include "SDK_Camera.h"
#include "SDK_Transform.h"
#include "SDK_ComputeTool.h"
#include "SDK_StringTool.h"
#include "SDK_Scene.h"

glm::mat4 TextMatrix;

void SDK::Text::Init(SDK::Font& FontResource) {
	FontPtr = &FontResource;
	LineLengthList.reserve(5);
}

void SDK::Text::SetRenderType(int Type) {
	RenderType = Type;
}

void SDK::Text::SetAlign(int AlignOpt) {
	TextAlign = AlignOpt;
}

void SDK::Text::SetLineGap(float Value) {
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

void SDK::Text::SetShadow(float OffsetX, float OffsetY, float Opacity, SDK::Color3 Color) {
	ShadowOffset.x = OffsetX;
	ShadowOffset.y = OffsetY;
	ShadowOpacity = Opacity;
	ShadowColor = Color;
}

void SDK::Text::SetColor(float R, float G, float B) {
	TextColor.r = R;
	TextColor.g = G;
	TextColor.b = B;
}

void SDK::Text::SetColor(SDK::Color3& Color) {
	TextColor.r = Color.r;
	TextColor.g = Color.g;
	TextColor.b = Color.b;
}

void SDK::Text::SetColorRGB(int R, int G, int B) {
	TextColor.r = (1.0f / 255.0f) * (float)R;
	TextColor.g = (1.0f / 255.0f) * (float)G;
	TextColor.b = (1.0f / 255.0f) * (float)B;
}

void SDK::Text::SetMacroColor(SDK::Color3& Color) {
	MacroColor = Color;
}

void SDK::Text::SetMacroColor(float R, float G, float B) {
	MacroColor.r = R;
	MacroColor.g = G;
	MacroColor.b = B;
}

void SDK::Text::SetMacroColorRGB(int R, int G, int B) {
	MacroColor.r = (1.0f / 255.0f) * (float)R;
	MacroColor.g = (1.0f / 255.0f) * (float)G;
	MacroColor.b = (1.0f / 255.0f) * (float)B;
}

void SDK::Text::Rotate(float Degree) {
	Rotation = -Degree;
}

void SDK::Text::SetOpacity(float Value) {
	TextOpacity = Value;
}

void SDK::Text::Render(SDK::Vector2& Position, float Size, const wchar_t* Fmt, ...) {
	if (!FontPtr || !FontPtr->InitializedState) {
		SDK::Scene.SetErrorScreen(ERROR_TYPE_NOT_CREATED_FONT_RENDER, SDK::Scene.ModeName());
		return;
	}

	if (Fmt == NULL)
		return;

		va_list Args{};
		va_start(Args, Fmt);
		int CurrentSize = vswprintf(nullptr, 0, Fmt, Args) + 1;

		if (PrevSize < CurrentSize) {
			TextVec.resize(CurrentSize);
			PrevSize = CurrentSize;
		}

		else if (PrevSize > CurrentSize) {
			TextVec.clear();
			PrevSize = CurrentSize;
		}

		vswprintf(TextVec.data(), CurrentSize, Fmt, Args);
		va_end(Args);

		InputText(TextVec, Position, Size);
}

void SDK::Text::Render(float X, float Y, float Size, const wchar_t* Fmt, ...) {
	if (!FontPtr || !FontPtr->InitializedState) {
		SDK::Scene.SetErrorScreen(ERROR_TYPE_NOT_CREATED_FONT_RENDER, SDK::Scene.ModeName());
		return;
	}

	if (Fmt == NULL)
		return;

	va_list Args{};
	va_start(Args, Fmt);
	int CurrentSize = vswprintf(nullptr, 0, Fmt, Args) + 1;

	if (PrevSize < CurrentSize) {
		TextVec.resize(CurrentSize);
		PrevSize = CurrentSize;
	}

	else if (PrevSize > CurrentSize) {
		TextVec.clear();
		PrevSize = CurrentSize;
	}

	vswprintf(TextVec.data(), CurrentSize, Fmt, Args);
	va_end(Args);

	InputText(TextVec, SDK::Vector2(X, Y), Size);
}

void SDK::Text::RenderString(SDK::Vector2& Position, float Size, std::string Str) {
	Render(Position.x, Position.y, Size, SDK::StringTool.Wstring(Str).c_str());
}

void SDK::Text::RenderString(float X, float Y, float Size, std::string Str) {
	Render(X, Y, Size, SDK::StringTool.Wstring(Str).c_str());
}

void SDK::Text::RenderWString(SDK::Vector2& Position, float Size, std::wstring WStr) {
	Render(Position.x, Position.y, Size, WStr.c_str());
}

void SDK::Text::RenderWString(float X, float Y, float Size, std::wstring WStr) {
	Render(X, Y, Size, WStr.c_str());
}

////////////////// private
void SDK::Text::InputText(std::vector<wchar_t>& Input, SDK::Vector2& Position, float Size) {
	CurrentText = Input.data();

	if (ShadowRenderCommand) {
		ShadowRenderState = true;
		RenderColor = ShadowColor;
		RenderOpacity = TextOpacity * ShadowOpacity;
		ProcessText((wchar_t*)CurrentText.c_str(), SDK::Vector2(Position.x + ShadowOffset.x * Size, Position.y + ShadowOffset.y * Size), Size);
	}

	ShadowRenderState = false;

	RenderColor = TextColor;
	RenderOpacity = TextOpacity;
	ProcessText((wchar_t*)CurrentText.c_str(), Position, Size);
}

void SDK::Text::ProcessText(wchar_t* Text, SDK::Vector2& Position, float Size) {
	CurrentLine = 0;
	TextRenderSize = Size;
	RenderPosition = Position;
	CurrentRenderOffset = SDK::Vector2(0.0, 0.0);

	if (CurrentText.compare(PrevText) != 0) {
		TextWordCount = wcslen(Text);
		FontPtr->UpdateGlyph(Text, TextWordCount);
		ComputeTextLength(Text);
		PrevText = CurrentText;
	}

	if (FixMiddleCommand && NumLine > 1) 
		FixMiddleOffset = ((TextLineGap + TextRenderSize) * (NumLine - 1)) * 0.5;
	else
		FixMiddleOffset = 0.0;

	TextLength = LineLengthList[0];
	
	switch (HeightAlign) {
	case HEIGHT_ALIGN_MIDDLE:
		RenderPosition.y -= TextRenderSize * 0.5;
		break;

	case HEIGHT_ALIGN_UNDER:
		RenderPosition.y -= TextRenderSize;
		break;
	}

	for (int i = 0; i < TextWordCount; ++i) {
		if (CheckCarrigeReturn(Text, i) || CheckColorMacro(Text, i)) 
			continue;

		TransformText();
		Camera.SetCamera(RenderType);
		PrepareRender();

		glPushAttrib(GL_LIST_BIT);
		glListBase(FontPtr->FontList);
		glCallLists(1, GL_UNSIGNED_SHORT, &Text[i]);
		glPopAttrib();

		if (Text[i] < FONT_LIST_GENERATE_SIZE)
			CurrentRenderOffset.x += FontPtr->TextGlyph[Text[i]].gmfCellIncX * (TextRenderSize / 1.0f);
	}
}

bool SDK::Text::CheckColorMacro(wchar_t*& Text, int& Index) {
	if (Text[Index] == L'\\' && Index + 1 < TextWordCount) {
		switch (Text[Index + 1]) {
		case L'P':
			if (!ShadowRenderState) RenderColor = MacroColor;
			Index += 1; return true;
		case L'R':
			if (!ShadowRenderState) RenderColor = SDK::Color3(1.0, 0.0, 0.0);
			Index += 1; return true;
		case L'G':
			if (!ShadowRenderState) RenderColor = SDK::Color3(0.0, 1.0, 0.0);
			Index += 1; return true;
		case L'B':
			if (!ShadowRenderState) RenderColor = SDK::Color3(0.0, 0.0, 1.0);
			Index += 1; return true;
		case L'Y':
			if (!ShadowRenderState) RenderColor = SDK::Color3(1.0, 1.0, 0.0);
			Index += 1; return true;
		case L'C':
			if (!ShadowRenderState) RenderColor = SDK::Color3(0.0, 1.0, 1.0);
			Index += 1; return true;
		case L'M':
			if (!ShadowRenderState) RenderColor = SDK::Color3(1.0, 0.0, 1.0);
			Index += 1; return true;
		case L'K':
			if (!ShadowRenderState) RenderColor = SDK::Color3(0.0, 0.0, 0.0);
			Index += 1; return true;
		case L'W':
			if (!ShadowRenderState) RenderColor = SDK::Color3(1.0, 1.0, 1.0);
			Index += 1; return true;
		case L'E':
			if (!ShadowRenderState) RenderColor = TextColor;
			Index += 1; return true;
		}
	}

	return false;
}

bool SDK::Text::CheckCarrigeReturn(wchar_t*& Text, int& Index) {
	if (Text[Index] == L'\n') {
		CurrentRenderOffset.x = 0.0;
		CurrentRenderOffset.y -= (TextLineGap + TextRenderSize);

		if (TextAlign != ALIGN_DEFAULT) {
			++CurrentLine;
			TextLength = LineLengthList[CurrentLine];
		}

		return true;
	}

	return false;
}

void SDK::Text::ComputeTextLength(wchar_t*& Text) {
	LineLengthList.clear();
	float CurrentLineLength{};

	for (int i = 0; i < wcslen(Text); ++i) {
		if (Text[i] == L'\n') {
			LineLengthList.emplace_back(CurrentLineLength);
			CurrentLineLength = 0.0f;
			continue;
		}

		else if (CheckColorMacro(Text, i))
			continue;

		unsigned int CharIndex = Text[i];
		if (CharIndex < FONT_LIST_GENERATE_SIZE)
			CurrentLineLength += FontPtr->TextGlyph[CharIndex].gmfCellIncX * (TextRenderSize / 1.0f);
	}

	if (CurrentLineLength > 0.0f)
		LineLengthList.emplace_back(CurrentLineLength);

	NumLine = LineLengthList.size();
}

void SDK::Text::TransformText() {
	SDK::Transform.Identity(TextMatrix);
	TextMatrix = translate(TextMatrix, SDK::Vector3(RenderPosition.x, RenderPosition.y, 0.0));

	if (Rotation != 0.0)
		TextMatrix = rotate(TextMatrix, glm::radians(Rotation), SDK::Vector3(0.0, 0.0, 1.0));
	if (FixMiddleCommand && NumLine > 1)
		TextMatrix = translate(TextMatrix, SDK::Vector3(0.0, FixMiddleOffset, 0.0));

	switch (TextAlign) {
	case ALIGN_DEFAULT:
		TextMatrix = translate(TextMatrix, SDK::Vector3(CurrentRenderOffset.x, CurrentRenderOffset.y, 0.0));
		break;

	case ALIGN_MIDDLE:
		TextMatrix = translate(TextMatrix, SDK::Vector3(-TextLength * 0.5, 0.0, 0.0));
		TextMatrix = translate(TextMatrix, SDK::Vector3(CurrentRenderOffset.x, CurrentRenderOffset.y, 0.0));
		break;

	case ALIGN_LEFT:
		TextMatrix = translate(TextMatrix, SDK::Vector3(-TextLength, 0.0, 0.0));
		TextMatrix = translate(TextMatrix, SDK::Vector3(CurrentRenderOffset.x, CurrentRenderOffset.y, 0.0));
		break;
	}

	TextMatrix = scale(TextMatrix, SDK::Vector3(TextRenderSize, TextRenderSize, 1.0));
}

void SDK::Text::PrepareRender() {
	glUseProgram(TEXT_SHADER);
	Camera.PrepareRender(SHADER_TYPE_TEXT);

	glUniform1f(TEXT_OPACITY_LOCATION, RenderOpacity);
	glUniform3f(TEXT_COLOR_LOCATION, RenderColor.r, RenderColor.g, RenderColor.b);
	glUniformMatrix4fv(TEXT_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(TextMatrix));
}