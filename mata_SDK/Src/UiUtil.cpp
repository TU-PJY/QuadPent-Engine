#include "UiUtil.h"

UI_Util SDK::UITool;

void UI_Util::ClampMatrixToCorner(int EdgePosition, glm::mat4& Matrix, GLfloat Width, GLfloat Height, GLfloat GapX, GLfloat GapY) {
	switch (EdgePosition) {
	case EDGE_LEFT_DOWN:
		Matrix = translate(Matrix, glm::vec3(SDK::RECT.lx + Width / 2.0 + GapX, SDK::RECT.ly + Height / 2.0 + GapY, 0.0));
		break;

	case EDGE_LEFT_UP:
		Matrix = translate(Matrix, glm::vec3(SDK::RECT.lx + Width / 2.0 + GapX, SDK::RECT.ry - Height / 2.0 - GapY, 0.0));
		break;

	case EDGE_RIGHT_DOWN:
		Matrix = translate(Matrix, glm::vec3(SDK::RECT.rx - Width / 2.0 - GapX, SDK::RECT.ly + Height / 2.0 + GapY, 0.0));
		break;

	case EDGE_RIGHT_UP:
		Matrix = translate(Matrix, glm::vec3(SDK::RECT.rx - Width / 2.0 - GapX, SDK::RECT.ry - Height / 2.0 - GapY, 0.0));
		break;
	}
}

void UI_Util::ClampMatrixToPosition(glm::mat4& Matrix, GLfloat PositionX, GLfloat PositionY, GLfloat Width, GLfloat Height, GLfloat GapX, GLfloat GapY) {
	Matrix = translate(Matrix, glm::vec3(PositionX + Width / 2.0 + GapX, PositionY + Height / 2.0 + GapY, 0.0));
}

void UI_Util::ClampPositionToCorner(int EdgePosition, GLfloat& PositionX, GLfloat& PositionY, GLfloat Width, GLfloat Height, GLfloat GapX, GLfloat GapY) {
	switch (EdgePosition) {
	case EDGE_LEFT_DOWN:
		PositionX = SDK::RECT.lx + Width / 2.0 + GapX;
		PositionY = SDK::RECT.ly + Height / 2.0 + GapY;
		break;

	case EDGE_LEFT_UP:
		PositionX = SDK::RECT.lx + Width / 2.0 + GapX;
		PositionY = SDK::RECT.ry - Height / 2.0 - GapY;
		break;

	case EDGE_RIGHT_DOWN:
		PositionX = SDK::RECT.rx - Width / 2.0 - GapX;
		PositionY = SDK::RECT.ly + Height / 2.0 + GapY;
		break;

	case EDGE_RIGHT_UP:
		PositionX = SDK::RECT.rx - Width / 2.0 - GapX;
		PositionY = SDK::RECT.ry - Height / 2.0 - GapY;
		break;
	}
}

void UI_Util::ClampPositionToCoord(GLfloat& ThisPositionX, GLfloat& ThisPositionY, GLfloat OtherPositionX, GLfloat OtherPositionY, GLfloat Width, GLfloat Height, GLfloat GapX, GLfloat GapY) {
	ThisPositionX = OtherPositionX + Width / 2.0 + GapX;
	ThisPositionY = OtherPositionY + Height / 2.0 + GapY;
}