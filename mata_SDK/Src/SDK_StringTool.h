#pragma once
#include "SDK_Header.h"
 
namespace SDK {
	class SDK_StringTool {
	public:
		void InputChar(std::string& Str, WPARAM Key, bool UseNewLine);
		void AddString(std::string& Str, std::string AddStr);
		void EraseString(std::string& Str);
		void RemoveString(std::string& Str, std::string RemoveStr);
		std::wstring Wstring(const std::string& Str);
	};

	extern SDK::SDK_StringTool StringTool;
}