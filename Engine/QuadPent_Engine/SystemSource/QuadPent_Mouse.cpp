#include "QuadPent_Mouse.h"
#include "QuadPent_Camera.h"

QP::QuadPent_Mouse QP::Mouse;

void QP::QuadPent_Mouse::ConvertPosition(int CursorX, int CursorY) {
	X = (float)((CursorX - (float)WindowWidth / 2.0) * (float)(1.0 / (float)(WindowWidth / 2.0))) * Aspect;
	Y = -(float)(CursorY - (float)WindowHeight / 2.0) * (float)(1.0 / (float)(WindowHeight / 2.0));
	DynamicX = (X + Camera.Position.x) / Camera.Zoom;
	DynamicY = (Y + Camera.Position.y) / Camera.Zoom;
}