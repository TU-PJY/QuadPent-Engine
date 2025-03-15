#pragma once
#include "QuadPent_Header.h"

namespace QP {
	class QuadPent_MoveTool {
	public:
		void MoveStraight(float& Position, int MoveDirection, float Speed, float FT);
		void MoveStraight(float& Position, float Speed, float FT);
		void MoveForward(float& X, float& Y, float Speed, int MoveDirection, float RotationValue, float FT, bool Plus90Option);
		void MoveForward(QP::Vector2& Position, float Speed, int MoveDirection, float RotationValue, float FrameTime, bool Plus90Option);
		void MoveForward(float& X, float& Y, float Speed, float RotationValue, float FT, bool Plus90Option);
		void MoveForward(QP::Vector2& Position, float Speed, float RotationValue, float FrameTime, bool Plus90Option);
	};

	extern QuadPent_MoveTool MoveTool;
}