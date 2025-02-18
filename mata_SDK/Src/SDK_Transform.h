#pragma once
#include "SDK_Header.h"
#include "SDK_ObjectValue.h"

namespace MSDK {
	class SDK_Transform {
	public:
		void Identity(glm::mat4& Matrix);
		void Move(glm::mat4& Matrix, float X, float Y);
		void Move(glm::mat4& Matrix, glm::vec2& Position);
		void Rotate(glm::mat4& Matrix, float Degree);
		void RotateRadians(glm::mat4& Matrix, float Radians);
		void Scale(glm::mat4& Matrix, float X, float Y);
		void Scale(glm::mat4& Matrix, glm::vec2& Size);
		void Tilt(glm::mat4& Matrix, float X, float Y);
		void Tilt(glm::mat4& Matrix, glm::vec2& Value);
		void ImageScale(glm::mat4& Matrix, float Width, float Height);
		void Flip(int FlipFlag);
		void UnitFlip(int FlipFlag);
		bool CheckIdentity(glm::mat4& Matrix);
		void RotateV(glm::mat4& Matrix, float Degree);
		void RotateH(glm::mat4& Matrix, float Degree);
	};

	extern MSDK::SDK_Transform Transform;
}