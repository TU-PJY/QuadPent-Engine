#include "QuadPent_Mouse.h"
#include "QuadPent_Camera.h"

QP::QuadPent_Mouse QP::Mouse;

void QP::QuadPent_Mouse::ConvertPosition(int CursorX, int CursorY) {
	X = (float)((CursorX - (float)QP::WindowWidth / 2.0) * (float)(1.0 / (float)(QP::WindowWidth / 2.0))) * QP::Aspect;
	Y = -(float)(CursorY - (float)QP::WindowHeight / 2.0) * (float)(1.0 / (float)(QP::WindowHeight / 2.0));
	DynamicX = (X + QP::Camera.Position.x) / QP::Camera.Zoom;
	DynamicY = (Y + QP::Camera.Position.y) / QP::Camera.Zoom;
}