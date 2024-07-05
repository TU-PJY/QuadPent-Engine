#pragma once
#include "GLHeader.h"
#include <cmath>

enum class MoveDir
{Negetive, Positive, Zero};

class PhysicsUtil {
private:
	GLfloat GravityAcc{};
	GLfloat MoveAcc{};
	GLfloat Speed{};

	int MoveDirection{};
	bool FallingState{};

public:
	void Fall(GLfloat& Position, GLfloat Gravity, float FT);
	void LandFloor(GLfloat& Position, GLfloat FloorHeight);
	void SetMove(MoveDir Direction);
	void SetFallingState();
	void AddGravityAcc(GLfloat AccValue);
	void BounceFloor(GLfloat& Position, GLfloat FloorHeight, GLfloat RebounceValue, GLfloat Threshold);
	void BounceWall(GLfloat RebounceValue);
	void LerpDcc(GLfloat& Speed, GLfloat Friction, float FT);
	void LerpAcc(GLfloat& Speed, GLfloat Dest, GLfloat AccValue, float FT);
	void LinearDcc(GLfloat& Speed, GLfloat Friction, float FT);
	void MoveUniform(GLfloat& Position, GLfloat Dest, float FT);
	void LinearAcc(GLfloat& Speed, GLfloat Dest, GLfloat AccValue, float FT);
};