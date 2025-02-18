#include "SDK_Physics.h"
#include "SDK_Math.h"
#include "SDK_EXTool.h"

void MSDK::Physics::SetGravity(float Value) {
	if (Value < 0.0) return;
	Gravity = Value;
}

void MSDK::Physics::SetFriction(float Value) {
	if (Value < 0.0) return;
	Friction = Value;
}

void MSDK::Physics::SetRebounceReduce(float Value) {
	if (Value < 0.0) return;
	RebounceReduce = Value;
}

void MSDK::Physics::SetMinimumRebounce(float Value) {
	if (Value < 0.0) return;
	MinRebounceValue = Value;
}

void MSDK::Physics::SetFloorHeight(float Value) {
	FloorHeight = Value;
}

void MSDK::Physics::SetHeightOffset(float Value) {
	HeightOffset = Value;
}

bool MSDK::Physics::GetFallingState() {
	return FallingState;
}

void MSDK::Physics::AddGravityAcc(float Value) {
	GravityAcc += Value;
	FallingState = true;
}

void MSDK::Physics::SetGravityAcc(float Value) {
	GravityAcc = Value;
	FallingState = true;
}

void MSDK::Physics::EnableFalling() {
	FallingState = true;
}

void MSDK::Physics::DisableFalling() {
	FallingState = false;
}

bool MSDK::Physics::CheckFloorCollision(float& HeightPosition) {
	if (HeightPosition <= FloorHeight + HeightOffset)
		return true;
	return false;
}

bool MSDK::Physics::CheckFloorCollision(glm::vec2& DestPosition) {
	if (DestPosition.y <= FloorHeight + HeightOffset)
		return true;
	return false;
}

void MSDK::Physics::LandOnFloor(float& HeightPosition) {
	HeightPosition = FloorHeight + HeightOffset;
	FallingState = false;
}

void MSDK::Physics::LandOnFloor(glm::vec2& DestPosition) {
	DestPosition.y = FloorHeight + HeightOffset;
	FallingState = false;
}

void MSDK::Physics::UpdateFalling(float& HeightPosition, float FrameTime) {
	if (FallingState) {
		GravityAcc -= Gravity * FrameTime;
		HeightPosition += GravityAcc * FrameTime;
		if (CheckFloorCollision(HeightPosition))
			LandOnFloor(HeightPosition);
	}
}

void MSDK::Physics::UpdateFalling(glm::vec2& DestPosition, float FrameTime) {
	if (FallingState) {
		GravityAcc -= Gravity * FrameTime;
		DestPosition.y += GravityAcc * FrameTime;
		if (CheckFloorCollision(DestPosition.y))
			LandOnFloor(DestPosition.y);
	}
}

void MSDK::Physics::UpdateBouncing(float& HeightPosition, float FrameTime) {
	if (FallingState) {
		GravityAcc -= Gravity * FrameTime;
		HeightPosition += GravityAcc * FrameTime;

		if (CheckFloorCollision(HeightPosition)) {
			HeightPosition = FloorHeight + HeightOffset;
			GravityAcc -= RebounceReduce;

			if (fabs(GravityAcc) <= MinRebounceValue) {
				GravityAcc = 0.0;
				LandOnFloor(HeightPosition);
			}
			else
				GravityAcc *= -1;
		}
	}
}

void MSDK::Physics::UpdateBouncing(glm::vec2& DestPosition, float FrameTime) {
	if (FallingState) {
		GravityAcc -= Gravity * FrameTime;
		DestPosition.y += GravityAcc * FrameTime;

		if (CheckFloorCollision(DestPosition.y)) {
			DestPosition.y = FloorHeight + HeightOffset;
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


void MSDK::Physics::LerpAcceleratation(float& Speed, float DestSpeed, float AccValue, float FT) {
	MSDK::Math.Lerp(Speed, DestSpeed, AccValue * (1.0 - Friction), FT);
}

void MSDK::Physics::LerpDeceleration(float& Speed, float FT) {
	MSDK::Math.Lerp(Speed, 0.0, Friction, FT);
}

void MSDK::Physics::LinearAcceleratation(float& Speed, float DestSpeed, float AccValue, float FT) {
	if (DestSpeed > 0.0) {
		Speed += AccValue * (1.0 - Friction) * FT;
		EXTool.ClampValue(Speed, DestSpeed, CLAMP_GREATER);
	}
	else {
		Speed -= AccValue * (1.0 - Friction) * FT;
		EXTool.ClampValue(Speed, DestSpeed, CLAMP_LESS);
	}
}

void MSDK::Physics::LinearDeceleration(float& Speed, float FT) {
	if (Speed > 0) {
		Speed -= Friction * FT;
		EXTool.ClampValue(Speed, 0.0, CLAMP_LESS);
	}
	else if (Speed < 0) {
		Speed += Friction * FT;
		EXTool.ClampValue(Speed, 0.0, CLAMP_GREATER);
	}
}

void MSDK::Physics::AdjustSpeedEqual(float& SpeedX, float& SpeedY) {
	float Length = std::sqrt(SpeedX * SpeedX + SpeedY * SpeedY);
	if (Length > 1.0) {
		SpeedX /= Length;
		SpeedY /= Length;
	}
}