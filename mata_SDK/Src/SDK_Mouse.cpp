#include "SDK_Mouse.h"
#include "SDK_Camera.h"

SDK_Mouse SDK::Mouse;

void SDK_Mouse::ConvertPosition(int X, int Y) {
	x = (GLfloat)((X - (GLfloat)SDK::WIDTH / 2.0) * (GLfloat)(1.0 / (GLfloat)(SDK::WIDTH / 2.0))) * SDK::ASPECT;
	y = -(GLfloat)(Y - (GLfloat)SDK::HEIGHT / 2.0) * (GLfloat)(1.0 / (GLfloat)(SDK::HEIGHT / 2.0));
}
