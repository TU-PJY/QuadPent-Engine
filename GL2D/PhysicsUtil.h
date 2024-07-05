#pragma once
#include "GLHeader.h"
#include <cmath>

class PhysicsUtil {
private:
	GLfloat GravityAcc;
	GLfloat MoveAcc;
	bool FallingState{};

public:
	void Falling(GLfloat& Position, GLfloat Gravity, float FT);
	void CheckFloor(GLfloat& Position, GLfloat FloorHeight);
	void SetFallingState();
	void SetGravityAcc(GLfloat AccValue);
	void Bounce(GLfloat& Position, GLfloat FloorHeight, GLfloat RebounceValue, GLfloat Threshold);
	void LerpDcc(GLfloat& Speed, GLfloat Friction, float FT);
	void LerpAcc(GLfloat& Speed, GLfloat Dest, GLfloat AccValue, float FT);
	void LinearDcc(GLfloat& Speed, GLfloat Friction, float FT);
	void LinearAcc(GLfloat& Speed, GLfloat Dest, GLfloat AccValue, float FT);
};