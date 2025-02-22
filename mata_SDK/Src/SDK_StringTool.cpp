#include "SDK_StringTool.h"

SDK::SDK_StringTool SDK::StringTool;

void SDK::SDK_StringTool::InputChar(std::string& Str, WPARAM Key, bool UseNewLine) {
	switch (Key) {
	case VK_TAB: case VK_ESCAPE:
		break;

	case VK_SPACE:
		AddString(Str, " ");
		break;

	case VK_BACK:
		EraseString(Str);
		break;

	case VK_RETURN:
		if (!UseNewLine) break;
		AddString(Str, "\n");
		break;

	default:
		Str += static_cast<char>(Key);
		break;
	}
}

void SDK::SDK_StringTool::AddString(std::string& Str, std::string AddStr) {
	Str += AddStr;
}

void SDK::SDK_StringTool::EraseString(std::string& Str) {
	if (!Str.empty()) {
		Str.pop_back();
		if (Str.back() == '\n')
			Str.pop_back();
	}
}

void SDK::SDK_StringTool::RemoveString(std::string& Str, std::string RemoveStr) {
	if (Str.empty())
		return;

	size_t Pos = Str.find(RemoveStr);
	while (Pos != std::string::npos) {
		Str.erase(Pos, RemoveStr.length());
		Pos = Str.find(RemoveStr);
	}
}

std::wstring SDK::SDK_StringTool::Wstring(const std::string& Str) {
	if (Str.empty())
		return L"";

	int SizeNeed = MultiByteToWideChar(CP_UTF8, 0, Str.c_str(), (int)Str.size(), NULL, 0);
	if (SizeNeed <= 0) 
		return L"";
	
	std::wstring Wstr(SizeNeed, 0);
	MultiByteToWideChar(CP_UTF8, 0, Str.c_str(), (int)Str.size(), &Wstr[0], SizeNeed);
	return Wstr;
}