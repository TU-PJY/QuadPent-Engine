#include "PhysicsUtil.h"
#include <cmath>

void PhysicsUtil::Falling(GLfloat& Position, GLfloat Gravity, float FT) {
	if (FallingState) {
		GravityAcc -= Gravity * FT;
		Position += GravityAcc * FT;
	}
}

void PhysicsUtil::CheckFloor(GLfloat& Position, GLfloat FloorHeight) {
	if (Position < FloorHeight) {
		Position = FloorHeight;
		GravityAcc = 0;
		FallingState = false;
	}
}

void PhysicsUtil::SetFallingState() {
	FallingState = true;
}

void PhysicsUtil::SetGravityAcc(GLfloat AccValue) {
	GravityAcc += AccValue;
	FallingState = true;
}

void PhysicsUtil::Bounce(GLfloat& Position, GLfloat FloorHeight, GLfloat RebounceValue, GLfloat Threshold) {
	if (Position < FloorHeight && FallingState) {
		Position = FloorHeight;

		if (GravityAcc <= Threshold) {
			GravityAcc = 0;
			FallingState = false;
		}
		else
			GravityAcc = -GravityAcc * RebounceValue;
	}
}

void PhysicsUtil::LerpDcc(GLfloat& Speed, GLfloat Friction, float FT) {
	Speed = std::lerp(Speed, 0.0, FT * Friction);
}

void PhysicsUtil::LerpAcc(GLfloat& Speed, GLfloat Dest, GLfloat AccValue, float FT) {
	Speed = std::lerp(Speed, Dest, FT * AccValue);
}

void PhysicsUtil::LinearDcc(GLfloat& Speed, GLfloat Friction, float FT) {
	Speed -= Friction * FT;
	if (Speed <= 0)
		Speed = 0;
}

void PhysicsUtil::LinearAcc(GLfloat& Speed, GLfloat Dest, GLfloat AccValue, float FT) {
	if (Dest > 0) {
		Speed += AccValue * FT;
		if (Speed >= Dest)
			Speed = Dest;
	}

	else if (Dest < 0) {
		Speed -= AccValue * FT;
		if (Speed <= Dest)
			Speed = Dest;
	}
}
