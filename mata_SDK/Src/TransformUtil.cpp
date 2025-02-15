#include "TransformUtil.h"
#include "ObjectValue.h"

TransformUtil SDK::Transform;

void TransformUtil::Identity(glm::mat4& Matrix) {
	Matrix = glm::mat4(1.0f);
}

void TransformUtil::Move(glm::mat4& Matrix, GLfloat X, GLfloat Y) {
	Matrix = translate(Matrix, glm::vec3(X, Y, 0.0));
}

void TransformUtil::Move(glm::mat4& Matrix, glm::vec2& Position) {
	Matrix = translate(Matrix, glm::vec3(Position.x, Position.y, 0.0));
}

void TransformUtil::Rotate(glm::mat4& Matrix, GLfloat Degree) {
	Matrix = rotate(Matrix, glm::radians(-Degree), glm::vec3(0.0, 0.0, 1.0));
}

void TransformUtil::RotateRadians(glm::mat4& Matrix, GLfloat Radians) {
	Matrix = rotate(Matrix, -Radians, glm::vec3(0.0, 0.0, 1.0));
}

void TransformUtil::RotateV(glm::mat4& Matrix, GLfloat Degree) {
	Matrix = rotate(Matrix, glm::radians(Degree), glm::vec3(1.0, 0.0, 0.0));
}

void TransformUtil::RotateH(glm::mat4& Matrix, GLfloat Degree) {
	Matrix = rotate(Matrix, glm::radians(Degree), glm::vec3(0.0, 1.0, 0.0));
}

void TransformUtil::Scale(glm::mat4& Matrix, GLfloat X, GLfloat Y) {
	Matrix = scale(Matrix, glm::vec3(X, Y, 1.0));
}

void TransformUtil::Scale(glm::mat4& Matrix, glm::vec2& Size) {
	Matrix = scale(Matrix, glm::vec3(Size.x, Size.y, 1.0));
}

void TransformUtil::Tilt(glm::mat4& Matrix, GLfloat X, GLfloat Y) {
	Matrix[1][0] += X;
	Matrix[0][1] += Y;
}

void TransformUtil::Tilt(glm::mat4& Matrix, glm::vec2& Value) {
	Matrix[1][0] += Value.x;
	Matrix[0][1] += Value.y;
}

void TransformUtil::ImageScale(glm::mat4& Matrix, GLfloat Width, GLfloat Height) {
	if (Width > Height)
		Matrix = glm::scale(Matrix, glm::vec3(1.0, Height / Width, 1.0));
	else if (Width < Height)
		Matrix = glm::scale(Matrix, glm::vec3(Width / Height, 1.0, 1.0));
}

void TransformUtil::Flip(int FlipFlag) {
	switch (FlipFlag) {
	case FLIP_TYPE_NONE:
		Identity(FlipMatrix);
		break;

	case FLIP_TYPE_H:
		RotateH(FlipMatrix, 180.0f);
		break;

	case FLIP_TYPE_V:
		RotateV(FlipMatrix, 180.0f);
		break;

	case FLIP_TYPE_HV:
		RotateH(FlipMatrix, 180.0f);
		RotateV(FlipMatrix, 180.0f);
		break;
	}
}

void TransformUtil::UnitFlip(int FlipFlag) {
	switch (FlipFlag) {
	case FLIP_TYPE_NONE:
		Identity(UnitFlipMatrix);
		break;

	case FLIP_TYPE_H:
		RotateH(UnitFlipMatrix, 180.0f);
		break;

	case FLIP_TYPE_V:
		RotateV(UnitFlipMatrix, 180.0f);
		break;

	case FLIP_TYPE_HV:
		RotateH(UnitFlipMatrix, 180.0f);
		RotateV(UnitFlipMatrix, 180.0f);
		break;
	}
}

bool TransformUtil::CheckIdentity(glm::mat4& Matrix) {
	if (std::memcmp(&Matrix, &IdentityMatrix, sizeof(glm::mat4)) != 0)
		return false;
	
	return true;
}