#include "SDK_Transform.h"
#include "SDK_ObjectValue.h"

SDK::SDK_Transform SDK::Transform;

void SDK::SDK_Transform::Identity(SDK::Matrix4& Matrix) {
	Matrix = SDK::Matrix4(1.0f);
}

bool SDK::SDK_Transform::CheckIdentity(SDK::Matrix4& Matrix) {
	if (std::memcmp(&Matrix, &SDK::IDENTITY_MATRIX, sizeof(SDK::Matrix4)) != 0)
		return false;

	return true;
}

void SDK::SDK_Transform::Move(float X, float Y, int MatrixType) {
	if (MatrixType == MATRIX_LOCAL)
		LocalMatrix = translate(LocalMatrix, SDK::Vector3(X, Y, 0.0));
	else if(MatrixType == MATRIX_GLOBAL)
		GlobalMatrix = translate(GlobalMatrix, SDK::Vector3(X, Y, 0.0));
}

void SDK::SDK_Transform::Move(SDK::Vector2& Position, int MatrixType) {
	if (MatrixType == MATRIX_LOCAL)
		LocalMatrix = translate(LocalMatrix, SDK::Vector3(Position.x, Position.y, 0.0));
	else if (MatrixType == MATRIX_GLOBAL)
		GlobalMatrix = translate(GlobalMatrix, SDK::Vector3(Position.x, Position.y, 0.0));
}

void SDK::SDK_Transform::Rotate(float Degree, int MatrixType) {
	if(MatrixType == MATRIX_LOCAL)
		LocalMatrix = rotate(LocalMatrix, glm::radians(-Degree), SDK::Vector3(0.0, 0.0, 1.0));
	else if (MatrixType == MATRIX_GLOBAL)
		GlobalMatrix = rotate(GlobalMatrix, glm::radians(-Degree), SDK::Vector3(0.0, 0.0, 1.0));
}

void SDK::SDK_Transform::RotateRadians(float Radians, int MatrixType) {
	if (MatrixType == MATRIX_LOCAL)
		LocalMatrix = rotate(LocalMatrix, -Radians, SDK::Vector3(0.0, 0.0, 1.0));
	else if (MatrixType == MATRIX_GLOBAL)
		GlobalMatrix = rotate(GlobalMatrix, -Radians, SDK::Vector3(0.0, 0.0, 1.0));
}

void SDK::SDK_Transform::RotateV(float Degree, int MatrixType) {
	if(MatrixType == MATRIX_LOCAL)
		LocalMatrix = rotate(LocalMatrix, glm::radians(Degree), SDK::Vector3(1.0, 0.0, 0.0));
	else if(MatrixType == MATRIX_GLOBAL)
		GlobalMatrix = rotate(GlobalMatrix, glm::radians(Degree), SDK::Vector3(1.0, 0.0, 0.0));
}

void SDK::SDK_Transform::RotateH(float Degree, int MatrixType) {
	if (MatrixType == MATRIX_LOCAL)
		LocalMatrix = rotate(LocalMatrix, glm::radians(Degree), SDK::Vector3(0.0, 1.0, 0.0));
	else if (MatrixType == MATRIX_GLOBAL)
		GlobalMatrix = rotate(GlobalMatrix, glm::radians(Degree), SDK::Vector3(0.0, 1.0, 0.0));
}

void SDK::SDK_Transform::Scale(float SizeX, float SizeY, int MatrixType) {
	if(MatrixType == MATRIX_LOCAL)
		LocalMatrix = scale(LocalMatrix, SDK::Vector3(SizeX, SizeY, 1.0));
	else if(MatrixType == MATRIX_GLOBAL)
		GlobalMatrix = scale(GlobalMatrix, SDK::Vector3(SizeX, SizeY, 1.0));
}

void SDK::SDK_Transform::Scale(SDK::Vector2& Size, int MatrixType) {
	if (MatrixType == MATRIX_LOCAL)
		LocalMatrix = scale(LocalMatrix, SDK::Vector3(Size.x, Size.y, 1.0));
	else if (MatrixType == MATRIX_GLOBAL)
		GlobalMatrix = scale(GlobalMatrix, SDK::Vector3(Size.x, Size.y, 1.0));
}

void SDK::SDK_Transform::Tilt(float X, float Y, int MatrixType) {
	if (MatrixType == MATRIX_LOCAL) {
		LocalMatrix[1][0] += X;
		LocalMatrix[0][1] += Y;
	}
	else if (MatrixType == MATRIX_GLOBAL) {
		GlobalMatrix[1][0] += X;
		GlobalMatrix[0][1] += Y;
	}
}

void SDK::SDK_Transform::Tilt(SDK::Vector2& Value, int MatrixType) {
	if (MatrixType == MATRIX_LOCAL) {
		LocalMatrix[1][0] += Value.x;
		LocalMatrix[0][1] += Value.y;
	}
	else if (MatrixType == MATRIX_GLOBAL) {
		GlobalMatrix[1][0] += Value.x;
		GlobalMatrix[0][1] += Value.y;
	}
}

void SDK::SDK_Transform::Flip(int FlipFlag, int MatrixType) {
	switch (FlipFlag) {
	case FLIP_TYPE_NONE:
		Identity(LocalFlipMatrix);
		break;

	case FLIP_TYPE_H:
		RotateH(180.0f, MatrixType);
		break;

	case FLIP_TYPE_V:
		RotateV(180.0f, MatrixType);
		break;

	case FLIP_TYPE_HV:
		RotateH(180.0f, MatrixType);
		RotateV(180.0f, MatrixType);
		break;
	}
}