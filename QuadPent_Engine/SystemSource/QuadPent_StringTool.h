#pragma once
#include "QuadPent_Header.h"
 
namespace QP {
	class QuadPent_StringTool {
	public:
		void InputWString(std::wstring& Str, QP::KeyEvent& Event, bool UseNewLine);
		void AddString(std::string& Str, std::string AddStr);
		void EraseString(std::string& Str);
		void RemoveString(std::string& Str, std::string RemoveStr);
		void AddWString(std::wstring& Str, std::wstring AddStr);
		void EraseWString(std::wstring& Str);
		void RemoveWstring(std::wstring& Str, std::wstring RemoveStr);
		std::wstring Wstring(const std::string& Str);
	};

	extern QP::QuadPent_StringTool StringTool;
}