#pragma once
#include "GLHeader.h"

class MouseUtil {
public:
	GLfloat x{}, y{};
	bool LButtonDown{}, RButtonDown{};
	void ConvertPosition(int X, int Y);
};
extern MouseUtil mouse;