#include "UiUtil.h"

void UI::ClampMatrixToCorner(Edge EdgePosition, glm::mat4& Matrix, GLfloat Width, GLfloat Height, GLfloat GapX, GLfloat GapY) {
	switch (EdgePosition) {
	case Edge::LeftDown:
		Matrix = translate(Matrix, glm::vec3(rect.lx + Width / 2.0 + GapX, rect.ly + Height / 2.0 + GapY, 0.0));
		break;

	case Edge::LeftUp:
		Matrix = translate(Matrix, glm::vec3(rect.lx + Width / 2.0 + GapX, rect.ry - Height / 2.0 - GapY, 0.0));
		break;

	case Edge::RightDown:
		Matrix = translate(Matrix, glm::vec3(rect.rx - Width / 2.0 - GapX, rect.ly + Height / 2.0 + GapY, 0.0));
		break;

	case Edge::RightUp:
		Matrix = translate(Matrix, glm::vec3(rect.rx - Width / 2.0 - GapX, rect.ry - Height / 2.0 - GapY, 0.0));
		break;
	}
}

void UI::ClampMatrixToPosition(glm::mat4& Matrix, GLfloat PositionX, GLfloat PositionY, GLfloat Width, GLfloat Height, GLfloat GapX, GLfloat GapY) {
	Matrix = translate(Matrix, glm::vec3(PositionX + Width / 2.0 + GapX, PositionY + Height / 2.0 + GapY, 0.0));
}

void UI::ClampPositionToCorner(Edge EdgePosition, GLfloat& PositionX, GLfloat& PositionY, GLfloat Width, GLfloat Height, GLfloat GapX, GLfloat GapY) {
	switch (EdgePosition) {
	case Edge::LeftDown:
		PositionX = rect.lx + Width / 2.0 + GapX;
		PositionY = rect.ly + Height / 2.0 + GapY;
		break;

	case Edge::LeftUp:
		PositionX = rect.lx + Width / 2.0 + GapX;
		PositionY = rect.ry - Height / 2.0 - GapY;
		break;

	case Edge::RightDown:
		PositionX = rect.rx - Width / 2.0 - GapX;
		PositionY = rect.ly + Height / 2.0 + GapY;
		break;

	case Edge::RightUp:
		PositionX = rect.rx - Width / 2.0 - GapX;
		PositionY = rect.ry - Height / 2.0 - GapY;
		break;
	}
}

void UI::ClampPositionToCoord(GLfloat& ThisPositionX, GLfloat& ThisPositionY, GLfloat OtherPositionX, GLfloat OtherPositionY, GLfloat Width, GLfloat Height, GLfloat GapX, GLfloat GapY) {
	ThisPositionX = OtherPositionX + Width / 2.0 + GapX;
	ThisPositionY = OtherPositionY + Height / 2.0 + GapY;
}