#include "FontUtil.h"
#include "StringUtil.h"

bool FontUtil::Import(const std::string& FontFilePath) {
    std::wstring Path = StringUtil::ConvW(FontFilePath);
    DWORD NumFonts{};

    NumFonts = AddFontResourceEx(Path.c_str(), FR_PRIVATE, 0);

    if (NumFonts == 0) {
        std::cout << "Failed to load font from file." << std::endl;
        return false;
    }
    else
        std::cout << "Loaded Font " << FontFilePath << std::endl;

    SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);

    return true;
}
