#include "QuadPent_Transform.h"
#include "QuadPent_ObjectValue.h"

QP::QuadPent_Transform QP::Transform;

void QP::QuadPent_Transform::Identity(QP::Matrix4& Matrix) {
	Matrix = QP::Matrix4(1.0f);
}

bool QP::QuadPent_Transform::CheckIdentity(QP::Matrix4& Matrix) {
	if (std::memcmp(&Matrix, &QP::IDENTITY_MATRIX, sizeof(QP::Matrix4)) != 0)
		return false;

	return true;
}

void QP::QuadPent_Transform::Move(float X, float Y, int MatrixType) {
	if (MatrixType == MATRIX_LOCAL)
		LocalMatrix = translate(LocalMatrix, QP::Vector3(X, Y, 0.0));
	else if(MatrixType == MATRIX_GLOBAL)
		GlobalMatrix = translate(GlobalMatrix, QP::Vector3(X, Y, 0.0));
}

void QP::QuadPent_Transform::Move(QP::Vector2& Position, int MatrixType) {
	if (MatrixType == MATRIX_LOCAL)
		LocalMatrix = translate(LocalMatrix, QP::Vector3(Position.x, Position.y, 0.0));
	else if (MatrixType == MATRIX_GLOBAL)
		GlobalMatrix = translate(GlobalMatrix, QP::Vector3(Position.x, Position.y, 0.0));
}

void QP::QuadPent_Transform::Rotate(float Degree, int MatrixType) {
	if(MatrixType == MATRIX_LOCAL)
		LocalMatrix = rotate(LocalMatrix, glm::radians(-Degree), QP::Vector3(0.0, 0.0, 1.0));
	else if (MatrixType == MATRIX_GLOBAL)
		GlobalMatrix = rotate(GlobalMatrix, glm::radians(-Degree), QP::Vector3(0.0, 0.0, 1.0));
}

void QP::QuadPent_Transform::RotateRadians(float Radians, int MatrixType) {
	if (MatrixType == MATRIX_LOCAL)
		LocalMatrix = rotate(LocalMatrix, -Radians, QP::Vector3(0.0, 0.0, 1.0));
	else if (MatrixType == MATRIX_GLOBAL)
		GlobalMatrix = rotate(GlobalMatrix, -Radians, QP::Vector3(0.0, 0.0, 1.0));
}

void QP::QuadPent_Transform::RotateV(float Degree, int MatrixType) {
	if(MatrixType == MATRIX_LOCAL)
		LocalMatrix = rotate(LocalMatrix, glm::radians(Degree), QP::Vector3(1.0, 0.0, 0.0));
	else if(MatrixType == MATRIX_GLOBAL)
		GlobalMatrix = rotate(GlobalMatrix, glm::radians(Degree), QP::Vector3(1.0, 0.0, 0.0));
}

void QP::QuadPent_Transform::RotateH(float Degree, int MatrixType) {
	if (MatrixType == MATRIX_LOCAL)
		LocalMatrix = rotate(LocalMatrix, glm::radians(Degree), QP::Vector3(0.0, 1.0, 0.0));
	else if (MatrixType == MATRIX_GLOBAL)
		GlobalMatrix = rotate(GlobalMatrix, glm::radians(Degree), QP::Vector3(0.0, 1.0, 0.0));
}

void QP::QuadPent_Transform::Scale(float SizeX, float SizeY, int MatrixType) {
	if(MatrixType == MATRIX_LOCAL)
		LocalMatrix = scale(LocalMatrix, QP::Vector3(SizeX, SizeY, 1.0));
	else if(MatrixType == MATRIX_GLOBAL)
		GlobalMatrix = scale(GlobalMatrix, QP::Vector3(SizeX, SizeY, 1.0));
}

void QP::QuadPent_Transform::Scale(QP::Vector2& Size, int MatrixType) {
	if (MatrixType == MATRIX_LOCAL)
		LocalMatrix = scale(LocalMatrix, QP::Vector3(Size.x, Size.y, 1.0));
	else if (MatrixType == MATRIX_GLOBAL)
		GlobalMatrix = scale(GlobalMatrix, QP::Vector3(Size.x, Size.y, 1.0));
}

void QP::QuadPent_Transform::Tilt(float X, float Y, int MatrixType) {
	if (MatrixType == MATRIX_LOCAL) {
		LocalMatrix[1][0] += X;
		LocalMatrix[0][1] += Y;
	}
	else if (MatrixType == MATRIX_GLOBAL) {
		GlobalMatrix[1][0] += X;
		GlobalMatrix[0][1] += Y;
	}
}

void QP::QuadPent_Transform::Tilt(QP::Vector2& Value, int MatrixType) {
	if (MatrixType == MATRIX_LOCAL) {
		LocalMatrix[1][0] += Value.x;
		LocalMatrix[0][1] += Value.y;
	}
	else if (MatrixType == MATRIX_GLOBAL) {
		GlobalMatrix[1][0] += Value.x;
		GlobalMatrix[0][1] += Value.y;
	}
}

void QP::QuadPent_Transform::Flip(int FlipFlag, int MatrixType) {
	if (MatrixType == MATRIX_LOCAL)
		LocalFlipFlag = FlipFlag;
	else if (MatrixType == MATRIX_GLOBAL)
		GlobalFlipFlag = FlipFlag;
}