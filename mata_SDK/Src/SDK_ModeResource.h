#pragma once
#include "SDK_Header.h"

// Declare the ptr of mode class's Start() function here.
// Map the mode start function pointer in ModeAttribute.cpp after declaration.
namespace SDK {
	struct SDK_MODE_RESOURCE {
		SDK::MODE_PTR Mode1;

	};

	extern SDK_MODE_RESOURCE MODE;
}