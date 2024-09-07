#pragma once
#include "EngineHeader.h"

namespace Math {
	void LookAt(GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY, GLfloat& RotationVar, GLfloat RotationSpeed, float FT);
	void LookAt(GLfloat Rotation, GLfloat& RotationVar, GLfloat RotationSpeed, float FT);
	GLfloat CalcDistance(GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY);
	GLfloat NormalizeDegree(GLfloat Degree);
	GLfloat CalculateShortestRotation(GLfloat CurrentDegree, GLfloat DegreeDest);
}