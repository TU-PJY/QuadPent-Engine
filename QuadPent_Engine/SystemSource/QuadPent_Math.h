#pragma once
#include "QuadPent_Header.h"

namespace QP {
	class QuadPent_Math {
	public:
		void LookAt(float& DestRotation, float FromX, float FromY, float ToX, float ToY, float RotationSpeed = 0.0, float FrameTime = 0.0);
		void LookAt(float& DestRotation, QP::Vector2& Position1, QP::Vector2& Position2, float Rotationpeed = 0.0, float FrameTime = 0.0);
		void LookAt(float& DestRotation, float Degree, float RotationSpeed = 0.0, float FrameTime = 0.0);
		float ComputeDistance(float FromX, float FromY, float ToX, float ToY);
		float ComputeDistance(QP::Vector2& Position1, QP::Vector2& Position2);
		QP::Vector2 ComputeMidPoint(QP::Vector2& Position1, QP::Vector2& Position2);
		void Lerp(float& DestValue, float Dest, float Speed, float FrameTime);
		void Lerp(QP::Vector2& DestValue, QP::Vector2& Dest, float Speed, float FrameTime);
		void Lerp(QP::Vector3& DestValue, QP::Vector3& Dest, float Speed, float FrameTime);
		float Computedegree(float FromX, float FromY, float ToX, float ToY);
		float Computedegree(QP::Vector2& Position1, QP::Vector2& Position2);
		float ComputeRadians(float FromX, float FromY, float ToX, float ToY);
		float ComputeRadians(QP::Vector2& Position1, QP::Vector2& Position2);
		void ComputeRay(QP::RayVector& RayVec, float FromX, float FromY, float ToX, float ToY);
		void ComputeRay(QP::RayVector& RayVec, QP::Vector2& From, QP::Vector2& To);
		void ComputeRayWithDegree(QP::RayVector& RayVec, float OriginX, float OriginY, float Degree, float LengthValue);
		void ComputeRayWithDegree(QP::RayVector& RayVec, QP::Vector2& Origin, float Degree, float LengthValue);
	private:
		float NormalizeDegree(float Degree);
		float ComputeShortestRotation(float CurrentDegree, float DegreeDest);
	};

	extern QP::QuadPent_Math Math;
}