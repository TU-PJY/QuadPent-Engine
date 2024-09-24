#include "AnimationUtil.h"
#include "MathUtil.h"

GLfloat LoopSinAnimation::Update(GLfloat MoveScale, GLfloat Speed, float FrameTime) {
	Num += Speed * FrameTime;
	return sin(Num) * MoveScale;
}

void LoopSinAnimation::SetValue(GLfloat Value) {
	Num = Value;
}

void LoopSinAnimation::Reset() {
	Num = 0.0;
}


GLfloat PopBounceAnimation::Update(GLfloat SizeDest, GLfloat ShakeValue, GLfloat SizeIncreaseSpeed, GLfloat ShakeSpeed, GLfloat ShakeReduceSpeed, float FrameTime) {
	Num3 += FrameTime * SizeIncreaseSpeed;

	if (Num3 >= SizeDest) {
		Num3 = SizeDest;
		Num1 += FrameTime * ShakeSpeed;
		Num2 = Math::Lerp(Num2, ShakeValue, ShakeReduceSpeed, FrameTime);
	}

	return Num3 + sin(Num1) * (ShakeValue - Num2);
}

void PopBounceAnimation::Reset(){
	Num1 = 0.0;
	Num2 = 0.0;
	Num3 = 0.0;
}