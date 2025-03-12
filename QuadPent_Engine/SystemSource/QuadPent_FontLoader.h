#pragma once
#include "QuadPent_Header.h"
namespace QP {
	class QuadPent_FontLoader {
	public:
		bool Load(const std::string& FontFilePath);
	};

	extern QP::QuadPent_FontLoader FontLoader;
}
