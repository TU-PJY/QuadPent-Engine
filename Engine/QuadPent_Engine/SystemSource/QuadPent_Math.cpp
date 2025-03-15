#include "QuadPent_Math.h"
#include "QuadPent_EXTool.h"

QP::QuadPent_Math QP::Math;

void QP::QuadPent_Math::LookAt(float& DestRotation, float FromX, float FromY, float ToX, float ToY, float RotationSpeed, float FrameTime) {
	float TargetAngle{}, ShortestAngle{};
	TargetAngle = Computedegree(FromX, FromY, ToX, ToY);
	TargetAngle = NormalizeDegree(TargetAngle);

	if (RotationSpeed > 0)
		ShortestAngle = std::lerp(ShortestAngle, ComputeShortestRotation(DestRotation, TargetAngle), FrameTime * RotationSpeed);
	else
		ShortestAngle = ComputeShortestRotation(DestRotation, TargetAngle);

	DestRotation = NormalizeDegree(DestRotation + ShortestAngle);
}

void QP::QuadPent_Math::LookAt(float& DestRotation, QP::Vector2& Position1, QP::Vector2& Position2, float RotationSpeed, float FrameTime) {
	float TargetAngle{}, ShortestAngle{};
	TargetAngle = Computedegree(Position1, Position2);
	TargetAngle = NormalizeDegree(TargetAngle);

	if (RotationSpeed > 0)
		ShortestAngle = std::lerp(ShortestAngle, ComputeShortestRotation(DestRotation, TargetAngle), FrameTime * RotationSpeed);
	else
		ShortestAngle = ComputeShortestRotation(DestRotation, TargetAngle);

	DestRotation = NormalizeDegree(DestRotation + ShortestAngle);
}

void QP::QuadPent_Math::LookAt(float& DestRotation, float Degree, float RotationSpeed, float FrameTime) {
	float TargetAngle{}, ShortestAngle{};
	TargetAngle = NormalizeDegree(Degree);

	if (RotationSpeed > 0)
		ShortestAngle = std::lerp(ShortestAngle, ComputeShortestRotation(DestRotation, TargetAngle), FrameTime * RotationSpeed);
	else
		ShortestAngle = ComputeShortestRotation(DestRotation, TargetAngle);

	DestRotation = NormalizeDegree(DestRotation + ShortestAngle);
}

void QP::QuadPent_Math::Lerp(float& DestValue, float Dest, float Speed, float FrameTime) {
	if (LERP_THRESHOLD > 0.0 && abs(DestValue - Dest) <= LERP_THRESHOLD) {
		DestValue = Dest;
		return;
	}

	float t = Speed * FrameTime;
	QP::EXTool.ClampValue(t, 1.0, CLAMP_GREATER);
	DestValue = std::lerp(DestValue, Dest, t);
}

void QP::QuadPent_Math::Lerp(QP::Vector2& DestValue, QP::Vector2& Dest, float Speed, float FrameTime) {
	float t = Speed * FrameTime;
	QP::EXTool.ClampValue(t, 1.0, CLAMP_GREATER);

	if (LERP_THRESHOLD > 0.0 && abs(DestValue.x - Dest.x) <= LERP_THRESHOLD) 
		DestValue.x = Dest.x;
	else 
		DestValue.x = std::lerp(DestValue.x, Dest.x, t);

	if (LERP_THRESHOLD > 0.0 && abs(DestValue.y - Dest.y) <= LERP_THRESHOLD)
		DestValue.y = Dest.y;
	else
		DestValue.y = std::lerp(DestValue.y, Dest.y, t);
}

void QP::QuadPent_Math::Lerp(QP::Vector3& DestValue, QP::Vector3& Dest, float Speed, float FrameTime) {
	float t = Speed * FrameTime;
	QP::EXTool.ClampValue(t, 1.0, CLAMP_GREATER);

	if (LERP_THRESHOLD > 0.0 && abs(DestValue.x - Dest.x) <= LERP_THRESHOLD)
		DestValue.x = Dest.x;
	else
		DestValue.x = std::lerp(DestValue.x, Dest.x, t);

	if (LERP_THRESHOLD > 0.0 && abs(DestValue.y - Dest.y) <= LERP_THRESHOLD)
		DestValue.y = Dest.y;
	else
		DestValue.y = std::lerp(DestValue.y, Dest.y, t);

	if (LERP_THRESHOLD > 0.0 && abs(DestValue.z - Dest.z) <= LERP_THRESHOLD)
		DestValue.z = Dest.z;
	else
		DestValue.z = std::lerp(DestValue.z, Dest.z, t);
}

float QP::QuadPent_Math::ComputeDistance(float FromX, float FromY, float ToX, float ToY) {
	return  std::sqrt(std::pow(FromX - ToX, 2) + std::pow(FromY - ToY, 2));
}

float QP::QuadPent_Math::ComputeDistance(QP::Vector2& Position1, QP::Vector2& Position2) {
	return  std::sqrt(std::pow(Position1.x - Position2.x, 2) + std::pow(Position1.y - Position2.y, 2));
}

QP::Vector2 QP::QuadPent_Math::ComputeMidPoint(QP::Vector2& Position1, QP::Vector2& Position2) {
	return QP::Vector2((Position1.x + Position2.x) / 2.0, (Position1.y + Position2.y) / 2.0);
}

float QP::QuadPent_Math::Computedegree(float FromX, float FromY, float ToX, float ToY) {
	return -NormalizeDegree(atan2(ToY - FromY, ToX - FromX) * (180.0 / XM_PI));
}
 
float QP::QuadPent_Math::Computedegree(QP::Vector2& Position1, QP::Vector2& Position2) {
	return -NormalizeDegree(atan2(Position2.y - Position1.y, Position2.x - Position1.x) * (180.0 / XM_PI));
}

float QP::QuadPent_Math::ComputeRadians(float FromX, float FromY, float ToX, float ToY) {
	return -atan2(ToY - FromY, ToX - FromX);
}

float QP::QuadPent_Math::ComputeRadians(QP::Vector2& Position1, QP::Vector2& Position2) {
	return -atan2(Position2.y - Position1.y, Position2.x - Position1.x);
}

void QP::QuadPent_Math::ComputeRay(QP::RayVector& RayVec, float FromX, float FromY, float ToX, float ToY) {
	XMVECTOR RayOrigin = XMVectorSet(FromX, FromY, 0.0, 0.0);
	float Angle = -ComputeRadians(FromX, FromY, ToX, ToY);
	float DirectionX = cos(Angle);
	float DirectionY = sin(Angle);

	XMVECTOR RayDirection = XMVectorSet(DirectionX, DirectionY, 0.0f, 0.0f);
	RayDirection = XMVector2Normalize(RayDirection);
	float RayLength = ComputeDistance(FromX, FromY, ToX, ToY);
	float RayDistance = 0.0;

	RayVec.Origin = RayOrigin;
	RayVec.Direction = RayDirection;
	RayVec.Distance = RayDistance;
	RayVec.Length = RayLength;
}

void QP::QuadPent_Math::ComputeRay(QP::RayVector& RayVec, QP::Vector2& From, QP::Vector2& To) {
	XMVECTOR RayOrigin = XMVectorSet(From.x, From.y, 0.0, 0.0);
	float Angle = -ComputeRadians(From.x, From.y, To.x, To.y);
	float DirectionX = cos(Angle);
	float DirectionY = sin(Angle);

	XMVECTOR RayDirection = XMVectorSet(DirectionX, DirectionY, 0.0f, 0.0f);
	RayDirection = XMVector2Normalize(RayDirection);
	float RayLength = ComputeDistance(From.x, From.y, To.x, To.y);
	float RayDistance = 0.0;

	RayVec.Origin = RayOrigin;
	RayVec.Direction = RayDirection;
	RayVec.Distance = RayDistance;
	RayVec.Length = RayLength;
}

void QP::QuadPent_Math::ComputeRayWithDegree(QP::RayVector& RayVec, float OriginX, float OriginY, float Degree, float LengthValue) {
	XMVECTOR RayOrigin = XMVectorSet(OriginX, OriginY, 0.0, 0.0);
	float Angle = -glm::radians(Degree);
	float DirectionX = cos(Angle);
	float DirectionY = sin(Angle);

	XMVECTOR RayDirection = XMVectorSet(DirectionX, DirectionY, 0.0f, 0.0f);
	RayDirection = XMVector2Normalize(RayDirection);
	float RayLength = LengthValue;
	float RayDistance = 0.0;

	RayVec.Origin = RayOrigin;
	RayVec.Direction = RayDirection;
	RayVec.Distance = RayDistance;
	RayVec.Length = RayLength;
}

void QP::QuadPent_Math::ComputeRayWithDegree(QP::RayVector& RayVec, QP::Vector2& Origin, float Degree, float LengthValue) {
	XMVECTOR RayOrigin = XMVectorSet(Origin.x, Origin.y, 0.0, 0.0);
	float Angle = -glm::radians(Degree);
	float DirectionX = cos(Angle);
	float DirectionY = sin(Angle);

	XMVECTOR RayDirection = XMVectorSet(DirectionX, DirectionY, 0.0f, 0.0f);
	RayDirection = XMVector2Normalize(RayDirection);
	float RayLength = LengthValue;
	float RayDistance = 0.0;

	RayVec.Origin = RayOrigin;
	RayVec.Direction = RayDirection;
	RayVec.Distance = RayDistance;
	RayVec.Length = RayLength;
}

////////////////////////////////// private

float QP::QuadPent_Math::NormalizeDegree(float Degree) {
	while (Degree < 0) Degree += 360;
	while (Degree >= 360) Degree -= 360;
	return Degree;
}

float QP::QuadPent_Math::ComputeShortestRotation(float CurrentDegree, float DegreeDest) {
	float Diff = DegreeDest - CurrentDegree;

	if (Diff > 180)
		Diff -= 360;
	else if (Diff < -180)
		Diff += 360;

	return Diff;
}