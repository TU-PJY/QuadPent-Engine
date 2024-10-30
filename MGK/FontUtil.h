#pragma once
#include "EngineHeader.h"

namespace FontUtil {
	bool Import(const std::string& fontFilePath, DWORD& numFonts);
	std::wstring ToWstr(const std::string& str);
}
