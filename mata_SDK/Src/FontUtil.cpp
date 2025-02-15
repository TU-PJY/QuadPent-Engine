#include "FontUtil.h"
#include "StringUtil.h"
#include "Scene.h"

FontUtil SDK::FontLoader;

bool FontUtil::Load(const std::string& FontFilePath) {
    std::wstring Path = SDK::StringTool.Wstring(FontFilePath);
    DWORD NumFonts{};

    NumFonts = AddFontResourceEx(Path.c_str(), FR_PRIVATE, 0);

    if (NumFonts == 0) {
        SDK::Scene.SetErrorScreen(ERROR_TYPE_FONT_LOAD, FontFilePath);
        return false;
    }
    else
        std::cout << "Loaded Font " << FontFilePath << std::endl;

    SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);

    return true;
}

bool FontUtil::LoadT(const std::string& FontFilePath) {
    std::wstring Path = SDK::StringTool.Wstring(FontFilePath);
    DWORD NumFonts{};

    NumFonts = AddFontResourceEx(Path.c_str(), FR_PRIVATE, 0);

    if (NumFonts == 0) {
        SDK::Scene.SetErrorScreen(ERROR_TYPE_FONT_LOAD, FontFilePath);
        return false;
    }
    else
        std::cout << "Loaded Font " << FontFilePath << std::endl;

    PostMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);

    return true;
}
