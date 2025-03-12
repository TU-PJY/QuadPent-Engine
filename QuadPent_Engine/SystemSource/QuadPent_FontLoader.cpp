#include "QuadPent_FontLoader.h"
#include "QuadPent_StringTool.h"
#include "QuadPent_Scene.h"

QP::QuadPent_FontLoader QP::FontLoader;

void QP::QuadPent_FontLoader::Load(const std::string& FontFilePath) {
    std::wstring Path = StringTool.Wstring(FontFilePath);
    DWORD NumFonts{};

    NumFonts = AddFontResourceEx(Path.c_str(), FR_PRIVATE, 0);

    if (NumFonts == 0) {
        QP::Scene.SetErrorScreen(ERROR_TYPE_FONT_LOAD, FontFilePath);
        return;
    }

    SendMessage(QP::System_HWND, WM_FONTCHANGE, 0, 0);
}

void QP::QuadPent_FontLoader::LoadT(const std::string& FontFilePath) {
    std::wstring Path = StringTool.Wstring(FontFilePath);
    DWORD NumFonts{};

    NumFonts = AddFontResourceEx(Path.c_str(), FR_PRIVATE, 0);

    if (NumFonts == 0) {
        QP::Scene.SetErrorScreen(ERROR_TYPE_FONT_LOAD, FontFilePath);
        return;
    }

    PostMessage(QP::System_HWND, WM_FONTCHANGE, 0, 0);
}