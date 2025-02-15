#include "SDK_UITool.h"

SDK_UITool SDK::UITool;

void SDK_UITool::ClampMatrixToCorner(int EdgePosition, glm::mat4& Matrix, GLfloat Width, GLfloat Height, GLfloat GapX, GLfloat GapY) {
	switch (EdgePosition) {
	case EDGE_LEFT_DOWN:
		Matrix = translate(Matrix, glm::vec3(SDK::RECT.LeftX + Width / 2.0 + GapX, SDK::RECT.LeftY + Height / 2.0 + GapY, 0.0));
		break;

	case EDGE_LEFT_UP:
		Matrix = translate(Matrix, glm::vec3(SDK::RECT.LeftX + Width / 2.0 + GapX, SDK::RECT.RightY - Height / 2.0 - GapY, 0.0));
		break;

	case EDGE_RIGHT_DOWN:
		Matrix = translate(Matrix, glm::vec3(SDK::RECT.RightX - Width / 2.0 - GapX, SDK::RECT.LeftY + Height / 2.0 + GapY, 0.0));
		break;

	case EDGE_RIGHT_UP:
		Matrix = translate(Matrix, glm::vec3(SDK::RECT.RightX - Width / 2.0 - GapX, SDK::RECT.RightY - Height / 2.0 - GapY, 0.0));
		break;
	}
}

void SDK_UITool::ClampMatrixToPosition(glm::mat4& Matrix, GLfloat PositionX, GLfloat PositionY, GLfloat Width, GLfloat Height, GLfloat GapX, GLfloat GapY) {
	Matrix = translate(Matrix, glm::vec3(PositionX + Width / 2.0 + GapX, PositionY + Height / 2.0 + GapY, 0.0));
}

void SDK_UITool::ClampPositionToCorner(int EdgePosition, GLfloat& PositionX, GLfloat& PositionY, GLfloat Width, GLfloat Height, GLfloat GapX, GLfloat GapY) {
	switch (EdgePosition) {
	case EDGE_LEFT_DOWN:
		PositionX = SDK::RECT.LeftX + Width / 2.0 + GapX;
		PositionY = SDK::RECT.LeftY + Height / 2.0 + GapY;
		break;

	case EDGE_LEFT_UP:
		PositionX = SDK::RECT.LeftX + Width / 2.0 + GapX;
		PositionY = SDK::RECT.RightY - Height / 2.0 - GapY;
		break;

	case EDGE_RIGHT_DOWN:
		PositionX = SDK::RECT.RightX - Width / 2.0 - GapX;
		PositionY = SDK::RECT.LeftY + Height / 2.0 + GapY;
		break;

	case EDGE_RIGHT_UP:
		PositionX = SDK::RECT.RightX - Width / 2.0 - GapX;
		PositionY = SDK::RECT.RightY - Height / 2.0 - GapY;
		break;
	}
}

void SDK_UITool::ClampPositionToCoord(GLfloat& ThisPositionX, GLfloat& ThisPositionY, GLfloat OtherPositionX, GLfloat OtherPositionY, GLfloat Width, GLfloat Height, GLfloat GapX, GLfloat GapY) {
	ThisPositionX = OtherPositionX + Width / 2.0 + GapX;
	ThisPositionY = OtherPositionY + Height / 2.0 + GapY;
}