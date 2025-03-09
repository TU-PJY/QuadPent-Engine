#pragma once
#include "SDK_Header.h"

namespace SDK {
	class SDK_Mouse {
	public:
		float X{}, Y{};
		float DynamicX{}, DynamicY{};
		void ConvertPosition(int CursorX, int CursorY);
	};

	extern SDK::SDK_Mouse Mouse;
}