#pragma once
#include "SDK_Header.h"

namespace MSDK {
	class SDK_Mouse {
	public:
		float x{}, y{};
		void ConvertPosition(int X, int Y);
	};

	extern MSDK::SDK_Mouse Mouse;
}