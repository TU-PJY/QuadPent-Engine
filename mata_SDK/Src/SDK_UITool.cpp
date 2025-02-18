#include "SDK_UITool.h"

MSDK::SDK_UITool MSDK::UITool;

void MSDK::SDK_UITool::ClampMatrixToCorner(int EdgePosition, glm::mat4& Matrix, float Width, float Height, float GapX, float GapY) {
	switch (EdgePosition) {
	case EDGE_LEFT_DOWN:
		Matrix = translate(Matrix, glm::vec3(MSDK::WindowRect.LeftX + Width / 2.0 + GapX, MSDK::WindowRect.LeftY + Height / 2.0 + GapY, 0.0));
		break;

	case EDGE_LEFT_UP:
		Matrix = translate(Matrix, glm::vec3(MSDK::WindowRect.LeftX + Width / 2.0 + GapX, MSDK::WindowRect.RightY - Height / 2.0 - GapY, 0.0));
		break;

	case EDGE_RIGHT_DOWN:
		Matrix = translate(Matrix, glm::vec3(MSDK::WindowRect.RightX - Width / 2.0 - GapX, MSDK::WindowRect.LeftY + Height / 2.0 + GapY, 0.0));
		break;

	case EDGE_RIGHT_UP:
		Matrix = translate(Matrix, glm::vec3(MSDK::WindowRect.RightX - Width / 2.0 - GapX, MSDK::WindowRect.RightY - Height / 2.0 - GapY, 0.0));
		break;
	}
}

void MSDK::SDK_UITool::ClampMatrixToPosition(glm::mat4& Matrix, float PositionX, float PositionY, float Width, float Height, float GapX, float GapY) {
	Matrix = translate(Matrix, glm::vec3(PositionX + Width / 2.0 + GapX, PositionY + Height / 2.0 + GapY, 0.0));
}

void MSDK::SDK_UITool::ClampPositionToCorner(int EdgePosition, float& PositionX, float& PositionY, float Width, float Height, float GapX, float GapY) {
	switch (EdgePosition) {
	case EDGE_LEFT_DOWN:
		PositionX = MSDK::WindowRect.LeftX + Width / 2.0 + GapX;
		PositionY = MSDK::WindowRect.LeftY + Height / 2.0 + GapY;
		break;

	case EDGE_LEFT_UP:
		PositionX = MSDK::WindowRect.LeftX + Width / 2.0 + GapX;
		PositionY = MSDK::WindowRect.RightY - Height / 2.0 - GapY;
		break;

	case EDGE_RIGHT_DOWN:
		PositionX = MSDK::WindowRect.RightX - Width / 2.0 - GapX;
		PositionY = MSDK::WindowRect.LeftY + Height / 2.0 + GapY;
		break;

	case EDGE_RIGHT_UP:
		PositionX = MSDK::WindowRect.RightX - Width / 2.0 - GapX;
		PositionY = MSDK::WindowRect.RightY - Height / 2.0 - GapY;
		break;
	}
}

void MSDK::SDK_UITool::ClampPositionToCoord(float& ThisPositionX, float& ThisPositionY, float OtherPositionX, float OtherPositionY, float Width, float Height, float GapX, float GapY) {
	ThisPositionX = OtherPositionX + Width / 2.0 + GapX;
	ThisPositionY = OtherPositionY + Height / 2.0 + GapY;
}