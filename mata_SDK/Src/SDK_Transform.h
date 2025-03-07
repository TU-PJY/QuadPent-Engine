#pragma once
#include "SDK_Header.h"
#include "SDK_ObjectValue.h"

namespace SDK {
	class SDK_Transform {
	public:
		void Identity(SDK::Matrix4& Matrix);
		bool CheckIdentity(SDK::Matrix4& Matrix);
		void Move(float X, float Y, int MatrixType = MATRIX_LOCAL);
		void Move(SDK::Vector2& Position, int MatrixType = MATRIX_LOCAL);
		void Rotate(float Degree, int MatrixType = MATRIX_LOCAL);
		void RotateRadians(float Radians, int MatrixType = MATRIX_LOCAL);
		void RotateV(float Degree, int MatrixType = MATRIX_LOCAL);
		void RotateH(float Degree, int MatrixType = MATRIX_LOCAL);
		void Scale(float SizeX, float SizeY, int MatrixType = MATRIX_LOCAL);
		void Scale(SDK::Vector2& Size, int MatrixType = MATRIX_LOCAL);
		void Tilt(float X, float Y, int MatrixType = MATRIX_LOCAL);
		void Tilt(SDK::Vector2& Value, int MatrixType = MATRIX_LOCAL);
		void Flip(int FlipFlag, int MatrixType = MATRIX_LOCAL);
	};

	extern SDK::SDK_Transform Transform;
}