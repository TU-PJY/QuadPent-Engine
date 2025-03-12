#include "QuadPent_FontLoader.h"
#include "QuadPent_StringTool.h"
#include "QuadPent_Scene.h"

QP::QuadPent_FontLoader QP::FontLoader;

bool QP::QuadPent_FontLoader::Load(const std::string& FontFilePath) {
    std::wstring Path = StringTool.Wstring(FontFilePath);
    DWORD NumFonts{};

    NumFonts = AddFontResourceEx(Path.c_str(), FR_PRIVATE, 0);

    if (NumFonts == 0) {
        QP::Scene.SetErrorScreen(ERROR_TYPE_FONT_LOAD, FontFilePath);
        return false;
    }

    PostMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);

    std::cout << "Loaded Font " << FontFilePath << std::endl;
    return true;
}
