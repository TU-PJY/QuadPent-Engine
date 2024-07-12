#pragma once
#include "MATA_HEADER.h"

class MouseUtil {
public:
	GLfloat x{}, y{};
	void ConvertPosition(int X, int Y);
	GLfloat ViewportX();
	GLfloat ViewportY();
};
extern MouseUtil mouse;