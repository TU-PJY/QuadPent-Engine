#include "StringUtil.h"

void StringUtil::AddString(std::string& Str, std::string& AddStr) {
	Str += AddStr;
}

void StringUtil::EraseString(std::string& Str) {
	Str.pop_back();
}

void StringUtil::RemoveString(std::string& Str, std::string& RemoveStr) {
	size_t Pos = Str.find(RemoveStr);
	while (Pos != std::string::npos) {
		Str.erase(Pos, RemoveStr.length());
		Pos = Str.find(RemoveStr);
	}
}

std::wstring StringUtil::Wstring(const std::string& Str) {
	int SizeNeed = MultiByteToWideChar(CP_UTF8, 0, &Str[0], (int)Str.size(), NULL, 0);
	std::wstring Wstr(SizeNeed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &Str[0], (int)Str.size(), &Wstr[0], SizeNeed);

	return Wstr;
}

const wchar_t* StringUtil::Wchar(const std::string& Str) {
	int SizeNeed = MultiByteToWideChar(CP_UTF8, 0, &Str[0], (int)Str.size(), NULL, 0);
	std::wstring Wstr(SizeNeed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &Str[0], (int)Str.size(), &Wstr[0], SizeNeed);

	return Wstr.c_str();
}