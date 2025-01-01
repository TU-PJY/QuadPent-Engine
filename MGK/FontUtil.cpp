#include "FontUtil.h"
#include "StringUtil.h"

FontUtil fontUtil;

bool FontUtil::Load(const std::string& FontFilePath, bool LoadInThread) {
    std::wstring Path = stringUtil.Wstring(FontFilePath);
    DWORD NumFonts{};

    NumFonts = AddFontResourceEx(Path.c_str(), FR_PRIVATE, 0);

    if (NumFonts == 0) {
        std::cout << "Failed to load font from file." << std::endl;
        return false;
    }
    else
        std::cout << "Loaded Font " << FontFilePath << std::endl;

    if(LoadInThread)
        PostMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
    else
        SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);

    return true;
}
