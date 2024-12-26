#pragma once
#include "EngineHeader.h"

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

extern ColorClipping colorClip;
extern AlphaClipping alphaClip;