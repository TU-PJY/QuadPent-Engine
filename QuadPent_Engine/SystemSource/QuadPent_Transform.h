#pragma once
#include "QuadPent_Header.h"
#include "QuadPent_ObjectValue.h"

namespace QP {
	class QuadPent_Transform {
	public:
		void Identity(QP::Matrix4& Matrix);
		bool CheckIdentity(QP::Matrix4& Matrix);
		void Move(float X, float Y, int MatrixType = MATRIX_LOCAL);
		void Move(QP::Vector2& Position, int MatrixType = MATRIX_LOCAL);
		void Rotate(float Degree, int MatrixType = MATRIX_LOCAL);
		void RotateRadians(float Radians, int MatrixType = MATRIX_LOCAL);
		void RotateV(float Degree, int MatrixType = MATRIX_LOCAL);
		void RotateH(float Degree, int MatrixType = MATRIX_LOCAL);
		void Scale(float SizeX, float SizeY, int MatrixType = MATRIX_LOCAL);
		void Scale(QP::Vector2& Size, int MatrixType = MATRIX_LOCAL);
		void Tilt(float X, float Y, int MatrixType = MATRIX_LOCAL);
		void Tilt(QP::Vector2& Value, int MatrixType = MATRIX_LOCAL);
		void Flip(int FlipFlag, int MatrixType = MATRIX_LOCAL);
	};

	extern QP::QuadPent_Transform Transform;
}