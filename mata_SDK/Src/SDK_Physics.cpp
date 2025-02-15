#include "SDK_Physics.h"
#include "SDK_Math.h"
#include "SDK_EXTool.h"

void SDK::Physics::SetGravity(GLfloat Value) {
	if (Value < 0.0) return;
	Gravity = Value;
}

void SDK::Physics::SetFriction(GLfloat Value) {
	if (Value < 0.0) return;
	Friction = Value;
}

void SDK::Physics::SetRebounceReduce(GLfloat Value) {
	if (Value < 0.0) return;
	RebounceReduce = Value;
}

void SDK::Physics::SetMinimumRebounce(GLfloat Value) {
	if (Value < 0.0) return;
	MinRebounceValue = Value;
}

void SDK::Physics::SetFloorHeight(GLfloat Value) {
	FloorHeight = Value;
}

void SDK::Physics::SetHeightOffset(GLfloat Value) {
	HeightOffset = Value;
}

bool SDK::Physics::GetFallingState() {
	return FallingState;
}

void SDK::Physics::AddGravityAcc(GLfloat Value) {
	GravityAcc += Value;
	FallingState = true;
}

void SDK::Physics::SetGravityAcc(GLfloat Value) {
	GravityAcc = Value;
	FallingState = true;
}

void SDK::Physics::EnableFalling() {
	FallingState = true;
}

void SDK::Physics::DisableFalling() {
	FallingState = false;
}

bool SDK::Physics::CheckFloorCollision(GLfloat& HeightPosition) {
	if (HeightPosition <= FloorHeight + HeightOffset)
		return true;
	return false;
}

bool SDK::Physics::CheckFloorCollision(glm::vec2& DestPosition) {
	if (DestPosition.y <= FloorHeight + HeightOffset)
		return true;
	return false;
}

void SDK::Physics::LandOnFloor(GLfloat& HeightPosition) {
	HeightPosition = FloorHeight + HeightOffset;
	FallingState = false;
}

void SDK::Physics::LandOnFloor(glm::vec2& DestPosition) {
	DestPosition.y = FloorHeight + HeightOffset;
	FallingState = false;
}

void SDK::Physics::UpdateFalling(GLfloat& HeightPosition, float FrameTime) {
	if (FallingState) {
		GravityAcc -= Gravity * FrameTime;
		HeightPosition += GravityAcc * FrameTime;
		if (CheckFloorCollision(HeightPosition))
			LandOnFloor(HeightPosition);
	}
}

void SDK::Physics::UpdateFalling(glm::vec2& DestPosition, float FrameTime) {
	if (FallingState) {
		GravityAcc -= Gravity * FrameTime;
		DestPosition.y += GravityAcc * FrameTime;
		if (CheckFloorCollision(DestPosition.y))
			LandOnFloor(DestPosition.y);
	}
}

void SDK::Physics::UpdateBouncing(GLfloat& HeightPosition, float FrameTime) {
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

void SDK::Physics::UpdateBouncing(glm::vec2& DestPosition, float FrameTime) {
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


void SDK::Physics::LerpAcceleratation(GLfloat& Speed, GLfloat DestSpeed, GLfloat AccValue, float FT) {
	SDK::Math.Lerp(Speed, DestSpeed, AccValue * (1.0 - Friction), FT);
}

void SDK::Physics::LerpDeceleration(GLfloat& Speed, float FT) {
	SDK::Math.Lerp(Speed, 0.0, Friction, FT);
}

void SDK::Physics::LinearAcceleratation(GLfloat& Speed, GLfloat DestSpeed, GLfloat AccValue, float FT) {
	if (DestSpeed > 0.0) {
		Speed += AccValue * (1.0 - Friction) * FT;
		EXTool.ClampValue(Speed, DestSpeed, CLAMP_GREATER);
	}
	else {
		Speed -= AccValue * (1.0 - Friction) * FT;
		EXTool.ClampValue(Speed, DestSpeed, CLAMP_LESS);
	}
}

void SDK::Physics::LinearDeceleration(GLfloat& Speed, float FT) {
	if (Speed > 0) {
		Speed -= Friction * FT;
		EXTool.ClampValue(Speed, 0.0, CLAMP_LESS);
	}
	else if (Speed < 0) {
		Speed += Friction * FT;
		EXTool.ClampValue(Speed, 0.0, CLAMP_GREATER);
	}
}

void SDK::Physics::AdjustSpeedEqual(GLfloat& SpeedX, GLfloat& SpeedY) {
	GLfloat Length = std::sqrt(SpeedX * SpeedX + SpeedY * SpeedY);
	if (Length > 1.0) {
		SpeedX /= Length;
		SpeedY /= Length;
	}
}