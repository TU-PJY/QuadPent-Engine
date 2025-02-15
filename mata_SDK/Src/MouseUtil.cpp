#include "MouseUtil.h"
#include "CameraUtil.h"

MouseUtil SDK::Mouse;

void MouseUtil::ConvertPosition(int X, int Y) {
	x = (GLfloat)((X - (GLfloat)SDK::WIDTH / 2.0) * (GLfloat)(1.0 / (GLfloat)(SDK::WIDTH / 2.0))) * SDK::ASPECT;
	y = -(GLfloat)(Y - (GLfloat)SDK::HEIGHT / 2.0) * (GLfloat)(1.0 / (GLfloat)(SDK::HEIGHT / 2.0));
}
