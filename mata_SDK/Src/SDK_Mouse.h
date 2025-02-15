#pragma once
#include "SDK_Header.h"

class SDK_Mouse {
public:
	GLfloat x{}, y{};
	void ConvertPosition(int X, int Y);
};

namespace SDK {
	extern SDK_Mouse Mouse;
}