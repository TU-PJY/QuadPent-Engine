#pragma once
#include "SDK_Header.h"
namespace MSDK {
	class SDK_FontLoader {
	public:
		bool Load(const std::string& FontFilePath);
		bool LoadT(const std::string& FontFilePath);
	};

	extern MSDK::SDK_FontLoader FontLoader;
}
