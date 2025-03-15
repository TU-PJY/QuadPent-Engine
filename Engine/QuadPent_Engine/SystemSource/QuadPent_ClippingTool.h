#pragma once
#include "QuadPent_Header.h"

namespace QP {
class QuadPent_ColorClipping {
public:
	void First();
	void Second();
	void End();
};

class QuadPent_AlphaClipping {
public:
	void First();
	void Second();
	void End();
};

	extern QuadPent_ColorClipping ColorClip;
	extern QuadPent_AlphaClipping AlphaClip;
}