#include "QuadPent_StringTool.h"

QP::QuadPent_StringTool QP::StringTool;

void QP::QuadPent_StringTool::InputWString(std::wstring& Str, QP::KeyEvent& Event, bool UseNewLine) {
	if (Event.Type == WM_KEYDOWN) {
		switch (Event.Key) {
		case VK_SPACE:
			AddWString(Str, L"");
			break;

		case VK_BACK:
			EraseWString(Str);
			break;

		case VK_RETURN:
			if (!UseNewLine) break;
			AddWString(Str, L"\n");
			break;
		}
	}

	else if(Event.Type == WM_CHAR && iswprint(Event.Key))
		Str.push_back(static_cast<wchar_t>(Event.Key));
}

void QP::QuadPent_StringTool::AddString(std::string& Str, std::string AddStr) {
	Str += AddStr;
}

void QP::QuadPent_StringTool::EraseString(std::string& Str) {
	if (!Str.empty()) {
		Str.pop_back();
		if (!Str.empty() && Str.back() == L'\n')
			Str.pop_back();
	}
}

void QP::QuadPent_StringTool::RemoveString(std::string& Str, std::string RemoveStr) {
	if (Str.empty())
		return;

	size_t Pos = Str.find(RemoveStr);
	while (Pos != std::string::npos) {
		Str.erase(Pos, RemoveStr.length());
		Pos = Str.find(RemoveStr);
	}
}

void QP::QuadPent_StringTool::AddWString(std::wstring& Str, std::wstring AddStr) {
	Str += AddStr;
}

void QP::QuadPent_StringTool::EraseWString(std::wstring& Str) {
	if (!Str.empty()) {
		Str.pop_back();
		if (!Str.empty() && (Str.back() == L'\n'))
			Str.pop_back();
	}
}

void QP::QuadPent_StringTool::RemoveWstring(std::wstring& Str, std::wstring RemoveStr) {
	if (Str.empty())
		return;

	size_t Pos = Str.find(RemoveStr);
	while (Pos != std::string::npos) {
		Str.erase(Pos, RemoveStr.length());
		Pos = Str.find(RemoveStr);
	}
}

std::wstring QP::QuadPent_StringTool::Wstring(const std::string& Str) {
	if (Str.empty())
		return L"";

	int SizeNeed = MultiByteToWideChar(CP_UTF8, 0, Str.c_str(), (int)Str.size(), NULL, 0);
	if (SizeNeed <= 0) 
		return L"";
	
	std::wstring Wstr(SizeNeed, 0);
	MultiByteToWideChar(CP_UTF8, 0, Str.c_str(), (int)Str.size(), &Wstr[0], SizeNeed);
	return Wstr;
}