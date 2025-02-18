#include "SDK_Mouse.h"
#include "SDK_Camera.h"

MSDK::SDK_Mouse MSDK::Mouse;

void MSDK::SDK_Mouse::ConvertPosition(int X, int Y) {
	x = (float)((X - (float)MSDK::WIDTH / 2.0) * (float)(1.0 / (float)(MSDK::WIDTH / 2.0))) * MSDK::ASPECT;
	y = -(float)(Y - (float)MSDK::HEIGHT / 2.0) * (float)(1.0 / (float)(MSDK::HEIGHT / 2.0));
}
