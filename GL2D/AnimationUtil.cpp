#include "AnimationUtil.h"
#include <cmath>

GLfloat SinAnimation::Animation(GLfloat MoveScale, float FrameTime) {
	Num += FrameTime;
	return sin(Num) * MoveScale;
}

GLfloat SinAnimation::Reset() {
	Num = 0;
}


GLfloat PopupAnimation::Animation(GLfloat SizeDest, GLfloat ShakeValue, GLfloat SizeIncreaseSoeed, GLfloat ShakeSpeed, GLfloat ShakeReduceSpeed, float FrameTime) {
	Num3 += FrameTime * SizeIncreaseSoeed;

	if (Num3 >= SizeDest) {
		Num3 = SizeDest;
		Num1 += FrameTime * ShakeSpeed;
		Num2 = std::lerp(Num2, ShakeValue, FrameTime * ShakeReduceSpeed);
	}

	return Num3 + sin(Num1) * (ShakeValue - Num2);
}

void PopupAnimation::Reset(){
	Num1 = 0;
	Num2 = 0;
	Num3 = 0;
}
