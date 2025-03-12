#pragma once
#include "QuadPent_Header.h"

namespace QP {
	class QuadPent_UITool {
	public:
		void ClampMatrixToCorner(int EdgePosition, QP::Matrix4& Matrix, float Width, float Height, float GapX, float GapY);
		void ClampMatrixToPosition(QP::Matrix4& Matrix, float PositionX, float PositionY, float Width, float Height, float GapX, float GapY);
		void ClampPositionToCorner(int EdgePosition, float& PositionX, float& PositionY, float Width, float Height, float GapX, float GapY);
		void ClampPositionToCoord(float& ThisPositionX, float& ThisPositionY, float OtherPositionX, float OtherPositionY, float Width, float Height, float GapX, float GapY);
	};

	extern QP::QuadPent_UITool UITool;
}