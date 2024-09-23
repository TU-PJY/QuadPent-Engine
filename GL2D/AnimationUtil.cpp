#include "AnimationUtil.h"
#include "MathUtil.h"

GLfloat LoopSinAnimation::Update(GLfloat MoveScale, GLfloat Speed, float FrameTime) {
	Num += Speed * FrameTime;

	if (Num >= 6.28)
		Num = 0.0;

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
	Num1 = 0;
	Num2 = 0;
	Num3 = 0;
}


GLfloat LerpSinAnimation::Update(GLfloat From, GLfloat To, GLfloat Speed, GLfloat AccValue, float FrameTime) {
	if (Increase) {
		Acc += FrameTime * AccValue;
		Num += Acc * Speed * FrameTime;

		if (Num >= 3.14 / 2.0) {
			Num = 3.14 / 2.0;
			Increase = false;
		}
	}

	else {
		Acc -= FrameTime * AccValue;
		Num -= Acc * Speed * FrameTime;
		if (Num <= 0)
			Num = 0;
		if (Acc <= 0)
			Acc = 0;
	}

	return From + sin(Num) * (To - From);
}

void LerpSinAnimation::Reset() {
	Acc = 0.0;
	Num = 0.0;
	Increase = true;
}
