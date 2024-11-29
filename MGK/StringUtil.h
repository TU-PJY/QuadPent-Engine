#pragma once
#include "EngineHeader.h"
 
namespace StringUtil {
	void AddString(std::string& Str, std::string& AddStr);
	void EraseString(std::string& Str);
	void RemoveString(std::string& Str, std::string& RemoveStr);
	std::wstring ConvW(const std::string& Str);
}