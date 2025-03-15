#pragma once
#include "QuadPent_Header.h"
namespace QP {
	class QuadPent_FontLoader {
	public:
		void Load(const std::string& FontFilePath);
		void LoadT(const std::string& FontFilePath);
	};

	extern QuadPent_FontLoader FontLoader;
}
