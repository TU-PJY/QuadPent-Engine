#pragma once
#include "SDK_Header.h"

class SDK_FontLoader {
public:
	bool Load(const std::string& FontFilePath);
	bool LoadT(const std::string& FontFilePath);
};

namespace SDK {
	extern SDK_FontLoader FontLoader;
}
