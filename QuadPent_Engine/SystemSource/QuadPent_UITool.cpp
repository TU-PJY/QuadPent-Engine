#include "QuadPent_UITool.h"

QP::QuadPent_UITool QP::UITool;

void QP::QuadPent_UITool::ClampMatrixToCorner(int EdgePosition, QP::Matrix4& Matrix, float Width, float Height, float GapX, float GapY) {
	switch (EdgePosition) {
	case EDGE_LEFT_DOWN:
		Matrix = translate(Matrix, Vector3(WindowRect.LeftX + Width / 2.0 + GapX, WindowRect.LeftY + Height / 2.0 + GapY, 0.0));
		break;

	case EDGE_LEFT_UP:
		Matrix = translate(Matrix, Vector3(WindowRect.LeftX + Width / 2.0 + GapX, WindowRect.RightY - Height / 2.0 - GapY, 0.0));
		break;

	case EDGE_RIGHT_DOWN:
		Matrix = translate(Matrix, Vector3(WindowRect.RightX - Width / 2.0 - GapX, WindowRect.LeftY + Height / 2.0 + GapY, 0.0));
		break;

	case EDGE_RIGHT_UP:
		Matrix = translate(Matrix, Vector3(WindowRect.RightX - Width / 2.0 - GapX, WindowRect.RightY - Height / 2.0 - GapY, 0.0));
		break;
	}
}

void QP::QuadPent_UITool::ClampMatrixToPosition(QP::Matrix4& Matrix, float PositionX, float PositionY, float Width, float Height, float GapX, float GapY) {
	Matrix = translate(Matrix, QP::Vector3(PositionX + Width / 2.0 + GapX, PositionY + Height / 2.0 + GapY, 0.0));
}

void QP::QuadPent_UITool::ClampPositionToCorner(int EdgePosition, float& PositionX, float& PositionY, float Width, float Height, float GapX, float GapY) {
	switch (EdgePosition) {
	case EDGE_LEFT_DOWN:
		PositionX = WindowRect.LeftX + Width / 2.0 + GapX;
		PositionY = WindowRect.LeftY + Height / 2.0 + GapY;
		break;

	case EDGE_LEFT_UP:
		PositionX = WindowRect.LeftX + Width / 2.0 + GapX;
		PositionY = WindowRect.RightY - Height / 2.0 - GapY;
		break;

	case EDGE_RIGHT_DOWN:
		PositionX = WindowRect.RightX - Width / 2.0 - GapX;
		PositionY = WindowRect.LeftY + Height / 2.0 + GapY;
		break;

	case EDGE_RIGHT_UP:
		PositionX = WindowRect.RightX - Width / 2.0 - GapX;
		PositionY = WindowRect.RightY - Height / 2.0 - GapY;
		break;
	}
}

void QP::QuadPent_UITool::ClampPositionToCoord(float& ThisPositionX, float& ThisPositionY, float OtherPositionX, float OtherPositionY, float Width, float Height, float GapX, float GapY) {
	ThisPositionX = OtherPositionX + Width / 2.0 + GapX;
	ThisPositionY = OtherPositionY + Height / 2.0 + GapY;
}