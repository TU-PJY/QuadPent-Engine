#include "SDK_Mouse.h"
#include "SDK_Camera.h"

SDK::SDK_Mouse SDK::Mouse;

void SDK::SDK_Mouse::ConvertPosition(int CursorX, int CursorY) {
	X = (float)((CursorX - (float)SDK::WindowWidth / 2.0) * (float)(1.0 / (float)(SDK::WindowWidth / 2.0))) * SDK::Aspect;
	Y = -(float)(CursorY - (float)SDK::WindowHeight / 2.0) * (float)(1.0 / (float)(SDK::WindowHeight / 2.0));
	DynamicX = (X + SDK::Camera.Position.x) / SDK::Camera.Zoom;
	DynamicY = (Y + SDK::Camera.Position.y) / SDK::Camera.Zoom;
}