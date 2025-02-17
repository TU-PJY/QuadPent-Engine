#pragma once
#include "SDK_Header.h"

namespace SDK {
	class SDK_Mouse {
	public:
		GLfloat x{}, y{};
		void ConvertPosition(int X, int Y);
	};

	extern SDK::SDK_Mouse Mouse;
}