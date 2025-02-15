#pragma once
#include "SDKHeader.h"

class SDK_ColorClipping {
public:
	void First();
	void Second();
	void End();
};

class SDK_AlphaClipping {
public:
	void First();
	void Second();
	void End();
};

namespace SDK {
	extern SDK_ColorClipping ColorClip;
	extern SDK_AlphaClipping AlphaClip;
}