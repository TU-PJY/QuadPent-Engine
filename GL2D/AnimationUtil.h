#pragma once
#include "MATA_HEADER.h"
#include <cmath>

class SinAnimation {
private:
	GLfloat Num{};

public:
	GLfloat Animation(GLfloat MoveScale, float FrameTime);
	GLfloat Reset();
};


class PopupAnimation {
private:
	GLfloat Num1{}, Num2{}, Num3{};

public:
	GLfloat Animation(GLfloat SizeDest, GLfloat ShakeValue, GLfloat SizeIncreaseSoeed, GLfloat ShakeSpeed, GLfloat ShakeReduceSpeed, float FrameTime);
	void Reset();
};