#include "QuadPent_Text.h"
#include "QuadPent_Camera.h"
#include "QuadPent_Transform.h"
#include "QuadPent_StringTool.h"
#include "QuadPent_Scene.h"

QP::Matrix4 TextMatrix;

void QP::Text::Init(QP::Font& FontResource) {
	FontPtr = &FontResource;
	LineLengthList.reserve(5);
}

void QP::Text::SetRenderType(int Type) {
	RenderType = Type;
}

void QP::Text::SetAlign(int AlignOpt) {
	TextAlign = AlignOpt;
}

void QP::Text::SetLineGap(float Value) {
	TextLineGap = Value;
}

void QP::Text::EnableFixMiddle() {
	FixMiddleCommand = true;
}

void QP::Text::DisableFixMiddle() {
	FixMiddleCommand = false;
}

void QP::Text::SetHeightAlign(int Type) {
	HeightAlign = Type;
}

void QP::Text::EnableShadow() {
	ShadowRenderCommand = true;
}

void QP::Text::DisableShadow() {
	ShadowRenderCommand = false;
}

void QP::Text::SetShadow(float OffsetX, float OffsetY, float Opacity, QP::Color3 Color) {
	ShadowOffset.x = OffsetX;
	ShadowOffset.y = OffsetY;
	ShadowOpacity = Opacity;
	ShadowColor = Color;
}

void QP::Text::SetColor(float R, float G, float B) {
	TextColor.r = R;
	TextColor.g = G;
	TextColor.b = B;
}

void QP::Text::SetColor(QP::Color3& Color) {
	TextColor.r = Color.r;
	TextColor.g = Color.g;
	TextColor.b = Color.b;
}

void QP::Text::SetColorRGB(int R, int G, int B) {
	TextColor.r = (1.0f / 255.0f) * (float)R;
	TextColor.g = (1.0f / 255.0f) * (float)G;
	TextColor.b = (1.0f / 255.0f) * (float)B;
}

void QP::Text::SetMacroColor(QP::Color3& Color) {
	MacroColor = Color;
}

void QP::Text::SetMacroColor(float R, float G, float B) {
	MacroColor.r = R;
	MacroColor.g = G;
	MacroColor.b = B;
}

void QP::Text::SetMacroColorRGB(int R, int G, int B) {
	MacroColor.r = (1.0f / 255.0f) * (float)R;
	MacroColor.g = (1.0f / 255.0f) * (float)G;
	MacroColor.b = (1.0f / 255.0f) * (float)B;
}

void QP::Text::Rotate(float Degree) {
	Rotation = -Degree;
}

void QP::Text::SetOpacity(float Value) {
	TextOpacity = Value;
}

void QP::Text::Render(QP::Vector2& Position, float Size, const wchar_t* Fmt, ...) {
	if (!FontPtr || !FontPtr->InitializedState) {
		QP::Scene.SetErrorScreen(ERROR_TYPE_NOT_CREATED_FONT_RENDER, QP::Scene.CurrentModeName());
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

void QP::Text::Render(float X, float Y, float Size, const wchar_t* Fmt, ...) {
	if (!FontPtr || !FontPtr->InitializedState) {
		QP::Scene.SetErrorScreen(ERROR_TYPE_NOT_CREATED_FONT_RENDER, QP::Scene.CurrentModeName());
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

	InputText(TextVec, QP::Vector2(X, Y), Size);
}

void QP::Text::RenderString(QP::Vector2& Position, float Size, std::string Str) {
	Render(Position.x, Position.y, Size, QP::StringTool.Wstring(Str).c_str());
}

void QP::Text::RenderString(float X, float Y, float Size, std::string Str) {
	Render(X, Y, Size, QP::StringTool.Wstring(Str).c_str());
}

void QP::Text::RenderWString(QP::Vector2& Position, float Size, std::wstring WStr) {
	Render(Position.x, Position.y, Size, WStr.c_str());
}

void QP::Text::RenderWString(float X, float Y, float Size, std::wstring WStr) {
	Render(X, Y, Size, WStr.c_str());
}

////////////////// private
void QP::Text::InputText(std::vector<wchar_t>& Input, QP::Vector2& Position, float Size) {
	CurrentText = Input.data();

	if (ShadowRenderCommand) {
		ShadowRenderState = true;
		RenderColor = ShadowColor;
		RenderOpacity = TextOpacity * ShadowOpacity;
		ProcessText((wchar_t*)CurrentText.c_str(), QP::Vector2(Position.x + ShadowOffset.x * Size, Position.y + ShadowOffset.y * Size), Size);
	}

	ShadowRenderState = false;

	RenderColor = TextColor;
	RenderOpacity = TextOpacity;
	ProcessText((wchar_t*)CurrentText.c_str(), Position, Size);
}

void QP::Text::ProcessText(wchar_t* Text, QP::Vector2& Position, float Size) {
	CurrentLine = 0;
	TextRenderSize = Size;
	RenderPosition = Position;
	CurrentRenderOffset = QP::Vector2(0.0, 0.0);

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

bool QP::Text::CheckColorMacro(wchar_t*& Text, int& Index) {
	if (Text[Index] == L'\\' && Index + 1 < TextWordCount) {
		switch (Text[Index + 1]) {
		case L'P':
			if (!ShadowRenderState) RenderColor = MacroColor;
			Index += 1; return true;
		case L'R':
			if (!ShadowRenderState) RenderColor = QP::Color3(1.0, 0.0, 0.0);
			Index += 1; return true;
		case L'G':
			if (!ShadowRenderState) RenderColor = QP::Color3(0.0, 1.0, 0.0);
			Index += 1; return true;
		case L'B':
			if (!ShadowRenderState) RenderColor = QP::Color3(0.0, 0.0, 1.0);
			Index += 1; return true;
		case L'Y':
			if (!ShadowRenderState) RenderColor = QP::Color3(1.0, 1.0, 0.0);
			Index += 1; return true;
		case L'C':
			if (!ShadowRenderState) RenderColor = QP::Color3(0.0, 1.0, 1.0);
			Index += 1; return true;
		case L'M':
			if (!ShadowRenderState) RenderColor = QP::Color3(1.0, 0.0, 1.0);
			Index += 1; return true;
		case L'K':
			if (!ShadowRenderState) RenderColor = QP::Color3(0.0, 0.0, 0.0);
			Index += 1; return true;
		case L'W':
			if (!ShadowRenderState) RenderColor = QP::Color3(1.0, 1.0, 1.0);
			Index += 1; return true;
		case L'E':
			if (!ShadowRenderState) RenderColor = TextColor;
			Index += 1; return true;
		}
	}

	return false;
}

bool QP::Text::CheckCarrigeReturn(wchar_t*& Text, int& Index) {
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

void QP::Text::ComputeTextLength(wchar_t*& Text) {
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

void QP::Text::TransformText() {
	QP::Transform.Identity(TextMatrix);
	TextMatrix = translate(TextMatrix, QP::Vector3(RenderPosition.x, RenderPosition.y, 0.0));

	if (Rotation != 0.0)
		TextMatrix = rotate(TextMatrix, glm::radians(Rotation), QP::Vector3(0.0, 0.0, 1.0));
	if (FixMiddleCommand && NumLine > 1)
		TextMatrix = translate(TextMatrix, QP::Vector3(0.0, FixMiddleOffset, 0.0));

	switch (TextAlign) {
	case ALIGN_DEFAULT:
		TextMatrix = translate(TextMatrix, QP::Vector3(CurrentRenderOffset.x, CurrentRenderOffset.y, 0.0));
		break;

	case ALIGN_MIDDLE:
		TextMatrix = translate(TextMatrix, QP::Vector3(-TextLength * 0.5, 0.0, 0.0));
		TextMatrix = translate(TextMatrix, QP::Vector3(CurrentRenderOffset.x, CurrentRenderOffset.y, 0.0));
		break;

	case ALIGN_LEFT:
		TextMatrix = translate(TextMatrix, QP::Vector3(-TextLength, 0.0, 0.0));
		TextMatrix = translate(TextMatrix, QP::Vector3(CurrentRenderOffset.x, CurrentRenderOffset.y, 0.0));
		break;
	}

	TextMatrix = scale(TextMatrix, QP::Vector3(TextRenderSize, TextRenderSize, 1.0));
}

void QP::Text::PrepareRender() {
	glUseProgram(TEXT_SHADER);
	Camera.PrepareRender(SHADER_TYPE_TEXT);

	glUniform1f(TEXT_OPACITY_LOCATION, RenderOpacity);
	glUniform3f(TEXT_COLOR_LOCATION, RenderColor.r, RenderColor.g, RenderColor.b);
	glUniformMatrix4fv(TEXT_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(TextMatrix));
}