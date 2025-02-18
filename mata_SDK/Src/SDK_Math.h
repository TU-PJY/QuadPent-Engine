#pragma once
#include "SDK_Header.h"

namespace MSDK {
	class SDK_Math {
	public:
		void LookAt(float& DestRotation, float FromX, float FromY, float ToX, float ToY, float RotationSpeed = 0.0, float FrameTime = 0.0);
		void LookAt(float& DestRotation, glm::vec2& Position1, glm::vec2& Position2, float Rotationpeed = 0.0, float FrameTime = 0.0);
		void LookAt(float& DestRotation, float Rotation, float RotationSpeed = 0.0, float FrameTime = 0.0);
		float ComputeDistance(float FromX, float FromY, float ToX, float ToY);
		float ComputeDistance(glm::vec2& Position1, glm::vec2& Position2);
		glm::vec2 ComputeMidPoint(glm::vec2& Position1, glm::vec2& Position2);
		void Lerp(float& DestValue, float Dest, float Speed, float FrameTime);
		void Lerp(glm::vec2& DestValue, glm::vec2& Dest, float Speed, float FrameTime);
		void Lerp(glm::vec3& DestValue, glm::vec3& Dest, float Speed, float FrameTime);
		float Computedegree(float FromX, float FromY, float ToX, float ToY);
		float Computedegree(glm::vec2& Position1, glm::vec2& Position2);
		float ComputeRadians(float FromX, float FromY, float ToX, float ToY);
		float ComputeRadians(glm::vec2& Position1, glm::vec2& Position2);
		MSDK::RayVector ComputeRay(float FromX, float FromY, float ToX, float ToY);
		MSDK::RayVector ComputeRay(glm::vec2& From, glm::vec2& To);
		MSDK::RayVector ComputeRayWithDegree(float OriginX, float OriginY, float Degree, float LengthValue = 0.0);
		MSDK::RayVector ComputeRayWithDegree(glm::vec2& Origin, float Degree, float LengthValue = 0.0);
	private:
		float NormalizeDegree(float Degree);
		float ComputeShortestRotation(float CurrentDegree, float DegreeDest);
	};

	extern MSDK::SDK_Math Math;
}