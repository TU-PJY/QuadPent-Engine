#pragma once
#include "SDKHeader.h"

class ColorClipping {
public:
	void First();
	void Second();
	void End();
};

class AlphaClipping {
public:
	void First();
	void Second();
	void End();
};

namespace SDK {
	extern ColorClipping ColorClip;
	extern AlphaClipping AlphaClip;
}