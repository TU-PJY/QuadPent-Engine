#pragma once
#include "SDK_Header.h"

namespace SDK {
	class SDK_Mouse {
	public:
		float x{}, y{};
		float LocalX{}, LocalY{};
		void ConvertPosition(int X, int Y);
	};

	extern SDK::SDK_Mouse Mouse;
}