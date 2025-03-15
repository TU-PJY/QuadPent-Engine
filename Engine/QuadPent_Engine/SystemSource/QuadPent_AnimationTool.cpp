#include "QuadPent_AnimationTool.h"
#include "QuadPent_Math.h"
#include "QuadPent_EXTool.h"

void QP::SinLoop::Update(float& Value, float MoveScale, float Speed, float FrameTime) {
	Num += Speed * FrameTime;
	Value = sin(Num) * MoveScale;
}

void QP::SinLoop::Update(QP::Vector2& Value, QP::Vector2& MoveScale, QP::Vector2& Speed, float FrameTime) {
	Num2.x += Speed.x * FrameTime;
	Num2.y += Speed.y * FrameTime;
	Value.x = sin(Num2.x) * MoveScale.x;
	Value.y = sin(Num2.y) * MoveScale.y;
}

void QP::SinLoop::SetValue(float Value) {
	Num = Value;
}

void QP::SinLoop::SetValue(QP::Vector2& Value) {
	Num2.x = Value.x;
	Num2.y = Value.y;
}

void QP::SinLoop::Reset() {
	Num = 0.0;
	Num2 = QP::Vector2(0.0, 0.0);
}



void QP::SinMove::SetMovePoint(float StartPoint, float EndPoint) {
	SingleStartPosition = StartPoint;
	SingleDistance = EndPoint - StartPoint;
}

void QP::SinMove::SetMovePoint(QP::Vector2& StartPoint, QP::Vector2& EndPoint) {
	StartPosition = StartPoint;
	Distance.x = EndPoint.x - StartPoint.x;
	Distance.y = EndPoint.y - StartPoint.y;
}

void QP::SinMove::SetValue(float Value) {
	Num = Value;
}

void QP::SinMove::Update(float& Value, float Speed, float FrameTime) {
	Num += Speed * FrameTime;
	QP::EXTool.ClampValue(Num, QP::Preset::MaxPositive, CLAMP_GREATER);
	Value = SingleStartPosition + (sin(Num) - sin(QP::Preset::MaxNegative)) * SingleDistance * 0.5;
}

void QP::SinMove::Update(QP::Vector2& Value, float Speed, float FrameTime) {
	Num += Speed * FrameTime;
	QP::EXTool.ClampValue(Num, QP::Preset::MaxPositive, CLAMP_GREATER);
	Value.x = StartPosition.x + (sin(Num) - sin(QP::Preset::MaxNegative)) * Distance.x * 0.5;
	Value.y = StartPosition.y + (sin(Num) - sin(QP::Preset::MaxNegative)) * Distance.y * 0.5;
}

void QP::SinMove::Reset() {
	Num = QP::Preset::MaxNegative;
}



void QP::PopBounce::Update(float& Value, float SizeDest, float ShakeScale, float SizeIncreaseSpeed, float ShakeSpeed, float ShakeReduceSpeed, float FrameTime) {
	Num3 += FrameTime * SizeIncreaseSpeed;

	if (Num3 >= SizeDest) {
		Num3 = SizeDest;
		Num1 += FrameTime * ShakeSpeed;
		Math.Lerp(Num2, ShakeScale, ShakeReduceSpeed, FrameTime);
	}

	Value = Num3 + sin(Num1) * (ShakeScale - Num2);
}

void QP::PopBounce::Reset(){
	Num1 = 0.0;
	Num2 = 0.0;
	Num3 = 0.0;
}



void QP::ReverseLerp::Update(float& Value, float Speed, float IncreaseSpeed, float FrameTime) {
	if (SingleMoveState) {
		Value += SingleVelocity * SingleDistance * FrameTime;
		SingleVelocity += SingleAcc * Speed  * FrameTime;
		SingleAcc += IncreaseSpeed * FrameTime;

		if (SingleStartPosition + SingleDistance < SingleStartPosition) {
			if (QP::EXTool.CheckClampValue(Value, SingleStartPosition + SingleDistance, CLAMP_LESS))
				SingleMoveState = false;
		}

		else if (SingleStartPosition + SingleDistance > SingleStartPosition) {
			if (QP::EXTool.CheckClampValue(Value, SingleStartPosition + SingleDistance, CLAMP_GREATER))
				SingleMoveState = false;
		}
	}
}

void QP::ReverseLerp::Update(QP::Vector2& Value, float Speed, float IncreaseSpeed, float FrameTime) {
	if (MoveState[0]) {
		Value.x += Velocity.x * Distance.x * FrameTime;
		Velocity.x += Acc.x * Speed * FrameTime;
		Acc.x += IncreaseSpeed * FrameTime;

		if (StartPosition.x + Distance.x < StartPosition.x) {
			if (QP::EXTool.CheckClampValue(Value.x, StartPosition.x + Distance.x, CLAMP_LESS))
				MoveState[0] = false;
		}

		if (StartPosition.x + Distance.x > StartPosition.x) {
			if (QP::EXTool.CheckClampValue(Value.x, StartPosition.x + Distance.x, CLAMP_GREATER))
				MoveState[0] = false;
		}
	}

	if (MoveState[1]) {
		Value.y += Velocity.y * Distance.y * FrameTime;
		Velocity.y += Acc.y * Speed * FrameTime;
		Acc.y += IncreaseSpeed * FrameTime;

		if (StartPosition.y + Distance.y < StartPosition.y) {
			if (QP::EXTool.CheckClampValue(Value.y, StartPosition.y + Distance.y, CLAMP_LESS))
				MoveState[1] = false;
		}

		if (StartPosition.y + Distance.y > StartPosition.y) {
			if (QP::EXTool.CheckClampValue(Value.y, StartPosition.y + Distance.y, CLAMP_GREATER))
				MoveState[1] = false;
		}
	}
}

void QP::ReverseLerp::SetMovePoint(float StartPoint, float EndPoint) {
	SingleStartPosition = StartPoint;
	SingleDistance = EndPoint - StartPoint;
}

void QP::ReverseLerp::SetMovePoint(QP::Vector2& StartPoint, QP::Vector2& EndPoint) {
	StartPosition = StartPoint;
	Distance.x = EndPoint.x - StartPoint.x;
	Distance.y = EndPoint.y - StartPoint.y;
}

void QP::ReverseLerp::Reset() {
	SingleVelocity = 0.0;
	SingleAcc = 0.0;
	SingleMoveState = true;

	Velocity = QP::Vector2(0.0, 0.0);
	Acc = QP::Vector2(0.0, 0.0);
	for (int i = 0; i < 2; i++)
		MoveState[i] = true;
}