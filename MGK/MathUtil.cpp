#include "MathUtil.h"
#include <cmath>

GLfloat NormalizeDegree(GLfloat Degree);
GLfloat ComputeShortestRotation(GLfloat CurrentDegree, GLfloat DegreeDest);

void Math::LookAt(GLfloat& DestRotation, GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY, GLfloat RotationSpeed, float FrameTime) {
	GLfloat TargetAngle{}, ShortestAngle{};
	TargetAngle = Computedegree(FromX, FromY, ToX, ToY) - 90.0;
	TargetAngle = NormalizeDegree(TargetAngle);

	if (RotationSpeed > 0)
		ShortestAngle = std::lerp(ShortestAngle, ComputeShortestRotation(DestRotation, TargetAngle), FrameTime * RotationSpeed);
	else
		ShortestAngle = ComputeShortestRotation(DestRotation, TargetAngle);

	DestRotation = NormalizeDegree(DestRotation + ShortestAngle);
}

void Math::LookAt(GLfloat& DestRotation, glm::vec2& Position1, glm::vec2& Position2, GLfloat RotationSpeed, float FrameTime) {
	GLfloat TargetAngle{}, ShortestAngle{};
	TargetAngle = Computedegree(Position1, Position2) - 90.0;
	TargetAngle = NormalizeDegree(TargetAngle);

	if (RotationSpeed > 0)
		ShortestAngle = std::lerp(ShortestAngle, ComputeShortestRotation(DestRotation, TargetAngle), FrameTime * RotationSpeed);
	else
		ShortestAngle = ComputeShortestRotation(DestRotation, TargetAngle);

	DestRotation = NormalizeDegree(DestRotation + ShortestAngle);
}

void Math::LookAt(GLfloat& RotationDest, GLfloat Rotation, GLfloat RotationSpeed, float FrameTime) {
	GLfloat TargetAngle{}, ShortestAngle{};
	TargetAngle = NormalizeDegree(Rotation);

	if (RotationSpeed > 0)
		ShortestAngle = std::lerp(ShortestAngle, ComputeShortestRotation(RotationDest, TargetAngle), FrameTime * RotationSpeed);
	else
		ShortestAngle = ComputeShortestRotation(RotationDest, TargetAngle);

	RotationDest = NormalizeDegree(RotationDest + ShortestAngle);
}

GLfloat Math::Lerp(GLfloat Value, GLfloat Dest, GLfloat Speed, float FrameTime) {
	GLfloat t = Speed * FrameTime;
	GLfloat ReturnValue = Value;

	if (t < 1.0 && t > 0.0)
		ReturnValue = std::lerp(ReturnValue, Dest, t);

	return ReturnValue;
}

void Math::UpdateLerp(GLfloat& DestValue, GLfloat Dest, GLfloat Speed, float FrameTime) {
	GLfloat t = Speed * FrameTime;

	if (t < 1.0 && t > 0.0)
		DestValue = std::lerp(DestValue, Dest, t);
}

GLfloat Math::ComputeDistance(GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY) {
	return  std::sqrt(std::pow(FromX - ToX, 2) + std::pow(FromY - ToY, 2));
}

GLfloat Math::ComputeDistance(glm::vec2& Position1, glm::vec2& Position2) {
	return  std::sqrt(std::pow(Position1.x - Position2.x, 2) + std::pow(Position1.y - Position2.y, 2));
}

glm::vec2 Math::ComputeMidPoint(glm::vec2& Position1, glm::vec2& Position2) {
	return glm::vec2((Position1.x + Position2.x) / 2.0, (Position1.y + Position2.y) / 2.0);
}

GLfloat Math::Computedegree(GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY) {
	return atan2(ToY - FromY, ToX - FromX) * (180.0 / 3.1415);
}
 
GLfloat Math::Computedegree(glm::vec2& Position1, glm::vec2& Position2) {
	return atan2(Position2.y - Position1.y, Position2.x - Position1.x) * (180.0 / 3.1415);
}

GLfloat Math::ComputeRadians(GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY) {
	return atan2(ToY - FromY, ToX - FromX);
}

GLfloat Math::ComputeRadians(glm::vec2& Position1, glm::vec2& Position2) {
	return atan2(Position2.y - Position1.y, Position2.x - Position1.x);
}


//////////////////////////////////



GLfloat NormalizeDegree(GLfloat Degree) {
	while (Degree < 0) Degree += 360;
	while (Degree >= 360) Degree -= 360;
	return Degree;
}

GLfloat ComputeShortestRotation(GLfloat CurrentDegree, GLfloat DegreeDest) {
	GLfloat Diff = DegreeDest - CurrentDegree;

	if (Diff > 180)
		Diff -= 360;
	else if (Diff < -180)
		Diff += 360;

	return Diff;
}