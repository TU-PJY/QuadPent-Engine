#pragma once
#include "SDK_Header.h"

namespace MSDK {
	class SDK_UITool {
	public:
		void ClampMatrixToCorner(int EdgePosition, glm::mat4& Matrix, float Width, float Height, float GapX, float GapY);
		void ClampMatrixToPosition(glm::mat4& Matrix, float PositionX, float PositionY, float Width, float Height, float GapX, float GapY);
		void ClampPositionToCorner(int EdgePosition, float& PositionX, float& PositionY, float Width, float Height, float GapX, float GapY);
		void ClampPositionToCoord(float& ThisPositionX, float& ThisPositionY, float OtherPositionX, float OtherPositionY, float Width, float Height, float GapX, float GapY);
	};

	extern MSDK::SDK_UITool UITool;
}