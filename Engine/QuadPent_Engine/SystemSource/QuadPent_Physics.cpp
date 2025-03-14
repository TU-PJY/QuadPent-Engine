#include "QuadPent_Physics.h"
#include "QuadPent_Math.h"
#include "QuadPent_EXTool.h"

void QP::Physics::SetGravity(float Value) {
	if (Value < 0.0) return;
	Gravity = Value;
}

void QP::Physics::SetFriction(float Value) {
	if (Value < 0.0) return;
	Friction = Value;
}

void QP::Physics::SetRebounceReduce(float Value) {
	if (Value < 0.0) return;
	RebounceReduce = Value;
}

void QP::Physics::SetMinimumRebounce(float Value) {
	if (Value < 0.0) return;
	MinRebounceValue = Value;
}

void QP::Physics::SetFloorHeight(float Value) {
	FloorHeight = Value;
}

void QP::Physics::SetHeightOffset(float Value) {
	HeightOffset = Value;
}

bool QP::Physics::GetFallingState() {
	return FallingState;
}

void QP::Physics::AddGravityAcc(float Value) {
	GravityAcc += Value;
	FallingState = true;
}

void QP::Physics::SetGravityAcc(float Value) {
	GravityAcc = Value;
	FallingState = true;
}

void QP::Physics::EnableFalling() {
	FallingState = true;
}

void QP::Physics::DisableFalling() {
	FallingState = false;
}

bool QP::Physics::CheckFloorCollision(float& HeightPosition) {
	if (HeightPosition <= FloorHeight + HeightOffset)
		return true;
	return false;
}

bool QP::Physics::CheckFloorCollision(QP::Vector2& DestPosition) {
	if (DestPosition.y <= FloorHeight + HeightOffset)
		return true;
	return false;
}

void QP::Physics::LandOnFloor(float& HeightPosition) {
	HeightPosition = FloorHeight + HeightOffset;
	FallingState = false;
}

void QP::Physics::LandOnFloor(QP::Vector2& DestPosition) {
	DestPosition.y = FloorHeight + HeightOffset;
	FallingState = false;
}

void QP::Physics::UpdateFalling(float& HeightPosition, float FrameTime) {
	if (FallingState) {
		GravityAcc -= Gravity * FrameTime;
		HeightPosition += GravityAcc * FrameTime;
		if (CheckFloorCollision(HeightPosition))
			LandOnFloor(HeightPosition);
	}
}

void QP::Physics::UpdateFalling(QP::Vector2& DestPosition, float FrameTime) {
	if (FallingState) {
		GravityAcc -= Gravity * FrameTime;
		DestPosition.y += GravityAcc * FrameTime;
		if (CheckFloorCollision(DestPosition.y))
			LandOnFloor(DestPosition.y);
	}
}

void QP::Physics::UpdateBouncing(float& HeightPosition, float FrameTime) {
	if (FallingState) {
		GravityAcc -= Gravity * FrameTime;
		HeightPosition += GravityAcc * FrameTime;

		if (CheckFloorCollision(HeightPosition)) {
			HeightPosition = FloorHeight + HeightOffset;
			GravityAcc *= -1;
			GravityAcc -= RebounceReduce;

			if (fabs(GravityAcc) <= MinRebounceValue) {
				GravityAcc = 0.0;
				LandOnFloor(HeightPosition);
			}
		}
	}
}

void QP::Physics::UpdateBouncing(QP::Vector2& DestPosition, float FrameTime) {
	if (FallingState) {
		GravityAcc -= Gravity * FrameTime;
		DestPosition.y += GravityAcc * FrameTime;

		if (CheckFloorCollision(DestPosition.y)) {
			DestPosition.y = FloorHeight + HeightOffset;
			GravityAcc *= -1;
			GravityAcc -= RebounceReduce;

			if (fabs(GravityAcc) <= MinRebounceValue) {
				GravityAcc = 0.0;
				LandOnFloor(DestPosition.y);
			}
			else 
				GravityAcc *= -1;
		}
	}
}


void QP::Physics::LerpAcceleratation(float& Speed, float DestSpeed, float AccValue, float FT) {
	Math.Lerp(Speed, DestSpeed, AccValue * (1.0 - Friction), FT);
}

void QP::Physics::LerpDeceleration(float& Speed, float FT) {
	Math.Lerp(Speed, 0.0, Friction, FT);
}

void QP::Physics::LinearAcceleratation(float& Speed, float DestSpeed, float AccValue, float FT) {
	if (DestSpeed > 0.0) {
		Speed += AccValue * (1.0 - Friction) * FT;
		EXTool.ClampValue(Speed, DestSpeed, CLAMP_GREATER);
	}
	else {
		Speed -= AccValue * (1.0 - Friction) * FT;
		EXTool.ClampValue(Speed, DestSpeed, CLAMP_LESS);
	}
}

void QP::Physics::LinearDeceleration(float& Speed, float FT) {
	if (Speed > 0) {
		Speed -= Friction * FT;
		EXTool.ClampValue(Speed, 0.0, CLAMP_LESS);
	}
	else if (Speed < 0) {
		Speed += Friction * FT;
		EXTool.ClampValue(Speed, 0.0, CLAMP_GREATER);
	}
}

void QP::Physics::AdjustSpeedEqual(float& SpeedX, float& SpeedY) {
	float Length = std::sqrt(SpeedX * SpeedX + SpeedY * SpeedY);
	if (Length > 1.0) {
		SpeedX /= Length;
		SpeedY /= Length;
	}
}