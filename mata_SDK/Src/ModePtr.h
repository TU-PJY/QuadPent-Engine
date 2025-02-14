#pragma once
#include "SDKHeader.h"

// Declare the ptr of mode class's Start() function here.
// Map the mode start function pointer in ModeAttribute.cpp after declaration.
class SDK_MODE {
public:
	MODE_PTR Mode1;
};

namespace SDK {
	extern SDK_MODE MODE;
}