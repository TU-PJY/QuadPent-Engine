#include "QuadPent_MoveTool.h"

QP::QuadPent_MoveTool QP::MoveTool;

void QP::QuadPent_MoveTool::MoveStraight(float& Position, int MoveDirection, float Speed, float FT) {
	Position += Speed * MoveDirection * FT;
}

void QP::QuadPent_MoveTool::MoveStraight(float& Position, float Speed, float FT) {
	Position += Speed * FT;
}

void QP::QuadPent_MoveTool::MoveForward(float& X, float& Y, float Speed, int MoveDirection, float RotationValue, float FT, bool Plus90Option) {
	if (Plus90Option) {
		X += Speed * cos(glm::radians(RotationValue + 90)) * MoveDirection * FT;
		Y += Speed * sin(glm::radians(RotationValue + 90)) * MoveDirection * FT;
	}
	else {
		X += Speed * cos(glm::radians(RotationValue)) * MoveDirection * FT;
		Y += Speed * sin(glm::radians(RotationValue)) * MoveDirection * FT;
	}
}

void QP::QuadPent_MoveTool::MoveForward(QP::Vector2& Position, float Speed, int MoveDirection, float RotationValue, float FrameTime, bool Plus90Option) {
	if (Plus90Option) {
		Position.x += Speed * cos(glm::radians(RotationValue + 90)) * MoveDirection * FrameTime;
		Position.y += Speed * sin(glm::radians(RotationValue + 90)) * MoveDirection * FrameTime;
	}
	else {
		Position.x += Speed * cos(glm::radians(RotationValue)) * MoveDirection * FrameTime;
		Position.y += Speed * sin(glm::radians(RotationValue)) * MoveDirection * FrameTime;
	}
}

void QP::QuadPent_MoveTool::MoveForward(float& X, float& Y, float Speed, float RotationValue, float FT, bool Plus90Option) {
	if (Plus90Option) {
		X += Speed * cos(glm::radians(RotationValue + 90)) * FT;
		Y += Speed * sin(glm::radians(RotationValue + 90)) * FT;
	}
	else {
		X += Speed * cos(glm::radians(RotationValue)) * FT;
		Y += Speed * sin(glm::radians(RotationValue)) * FT;
	}
}

void QP::QuadPent_MoveTool::MoveForward(QP::Vector2& Position, float Speed, float RotationValue, float FrameTime, bool Plus90Option) {
	if (Plus90Option) {
		Position.x += Speed * cos(glm::radians(RotationValue + 90)) * FrameTime;
		Position.y += Speed * sin(glm::radians(RotationValue + 90)) * FrameTime;
	}
	else {
		Position.x += Speed * cos(glm::radians(RotationValue)) * FrameTime;
		Position.y += Speed * sin(glm::radians(RotationValue)) * FrameTime;
	}
}