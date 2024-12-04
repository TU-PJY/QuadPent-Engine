#pragma once
#include "EngineHeader.h"

namespace Math {
	void LookAt(GLfloat& RotationVar, GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY, GLfloat RotationSpeed, float FrameTime);
	void LookAt(GLfloat& RotationVar, glm::vec2 Position1, glm::vec2 Position2, GLfloat Rotationpeed, float FrameTime);
	void LookAt(GLfloat& RotationVar, GLfloat Rotation, GLfloat RotationSpeed, float FT);
	GLfloat CalcDistance(GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY);
	GLfloat CalcDistance(glm::vec2 Position1, glm::vec2 Position2);
	glm::vec2 CalcMidPoint(glm::vec2 Position1, glm::vec2 Position2);
	GLfloat Lerp(GLfloat Value, GLfloat Dest, GLfloat Speed, float FrameTime);
	void UpdateLerp(GLfloat& Value, GLfloat Dest, GLfloat Speed, float FrameTime);
	GLfloat CalcDegree(GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY);
	GLfloat CalcDegree(glm::vec2 Position1, glm::vec2 Position2);
	GLfloat CalcRadians(GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY);
	GLfloat CalcRadians(glm::vec2 Position1, glm::vec2 Position2);
}