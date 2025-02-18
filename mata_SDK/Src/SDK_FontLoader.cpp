#include "SDK_FontLoader.h"
#include "SDK_StringTool.h"
#include "SDK_Scene.h"

MSDK::SDK_FontLoader MSDK::FontLoader;

bool MSDK::SDK_FontLoader::Load(const std::string& FontFilePath) {
    std::wstring Path = MSDK::StringTool.Wstring(FontFilePath);
    DWORD NumFonts{};

    NumFonts = AddFontResourceEx(Path.c_str(), FR_PRIVATE, 0);

    if (NumFonts == 0) {
        MSDK::Scene.SetErrorScreen(ERROR_TYPE_FONT_LOAD, FontFilePath);
        return false;
    }
    else
        std::cout << "Loaded Font " << FontFilePath << std::endl;

    SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);

    return true;
}

bool MSDK::SDK_FontLoader::LoadT(const std::string& FontFilePath) {
    std::wstring Path = StringTool.Wstring(FontFilePath);
    DWORD NumFonts{};

    NumFonts = AddFontResourceEx(Path.c_str(), FR_PRIVATE, 0);

    if (NumFonts == 0) {
        MSDK::Scene.SetErrorScreen(ERROR_TYPE_FONT_LOAD, FontFilePath);
        return false;
    }
    else
        std::cout << "Loaded Font " << FontFilePath << std::endl;

    PostMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);

    return true;
}
