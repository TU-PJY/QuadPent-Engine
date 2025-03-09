#include "SDK_Mouse.h"
#include "SDK_Camera.h"

SDK::SDK_Mouse SDK::Mouse;

void SDK::SDK_Mouse::ConvertPosition(int X, int Y) {
	x = (float)((X - (float)SDK::WindowWidth / 2.0) * (float)(1.0 / (float)(SDK::WindowWidth / 2.0))) * SDK::Aspect;
	y = -(float)(Y - (float)SDK::WindowHeight / 2.0) * (float)(1.0 / (float)(SDK::WindowHeight / 2.0));
	DynamicX = (x + SDK::Camera.Position.x) / SDK::Camera.Zoom;
	DynamicY = (y + SDK::Camera.Position.y) / SDK::Camera.Zoom;
}
