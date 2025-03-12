#pragma once
#include "QuadPent_Header.h"

namespace QP {
	class QuadPent_Mouse {
	public:
		float X{}, Y{};
		float DynamicX{}, DynamicY{};
		void ConvertPosition(int CursorX, int CursorY);
	};

	extern QP::QuadPent_Mouse Mouse;
}