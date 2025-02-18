#include "SDK_AnimationTool.h"
#include "SDK_Math.h"
#include "SDK_EXTool.h"

void MSDK::SinLoop::Update(float& Value, float MoveScale, float Speed, float FrameTime) {
	Num += Speed * FrameTime;
	Value = sin(Num) * MoveScale;
}

void MSDK::SinLoop::SetValue(float Value) {
	Num = Value;
}

void MSDK::SinLoop::Reset() {
	Num = 0.0;
}


void MSDK::PopBounce::Update(float& Value, float SizeDest, float ShakeScale, float SizeIncreaseSpeed, float ShakeSpeed, float ShakeReduceSpeed, float FrameTime) {
	Num3 += FrameTime * SizeIncreaseSpeed;

	if (Num3 >= SizeDest) {
		Num3 = SizeDest;
		Num1 += FrameTime * ShakeSpeed;
		Math.Lerp(Num2, ShakeScale, ShakeReduceSpeed, FrameTime);
	}

	Value = Num3 + sin(Num1) * (ShakeScale - Num2);
}

void MSDK::PopBounce::Reset(){
	Num1 = 0.0;
	Num2 = 0.0;
	Num3 = 0.0;
}


void MSDK::ReverseLerp::Update(float& Value, float Dest, float Speed, float IncreaseSpeed, float FrameTime) {
	float ReturnValue = Value;
	float Diff = Dest - Value;

	if (MoveState) {
		if (std::abs(Diff) < std::abs(Velocity)) {
			ReturnValue = Dest;
			Velocity = 0.0;
			Acc = 0.0;
			MoveState = false;
		}
		else {
			ReturnValue += Velocity * (Diff > 0 ? 1 : -1);
			Velocity += Acc * Speed * FrameTime;
			Acc += IncreaseSpeed * IncreaseSpeed * FrameTime;
		}
	}

	Value = ReturnValue;
}

void MSDK::ReverseLerp::Reset() {
	Velocity = 0.0;
	Acc = 0.0;
	MoveState = true;
}
