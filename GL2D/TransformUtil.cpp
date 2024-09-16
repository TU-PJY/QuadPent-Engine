#include "TransformUtil.h"

void Transform::Move(glm::mat4& Matrix, GLfloat X, GLfloat Y) {
	Matrix = translate(Matrix, glm::vec3(X, Y, 0.0));
}

void Transform::Rotate(glm::mat4& Matrix, GLfloat Degree) {
	Matrix = rotate(Matrix, glm::radians(Degree), glm::vec3(0.0, 0.0, 1.0));
}

void Transform::RotateRad(glm::mat4& Matrix, GLfloat Radians) {
	Matrix = rotate(Matrix, Radians, glm::vec3(0.0, 0.0, 1.0));
}

void Transform::RotateV(glm::mat4& Matrix, GLfloat Degree) {
	Matrix = rotate(Matrix, glm::radians(Degree), glm::vec3(1.0, 0.0, 0.0));
}

void Transform::RotateH(glm::mat4& Matrix, GLfloat Degree) {
	Matrix = rotate(Matrix, glm::radians(Degree), glm::vec3(0.0, 1.0, 0.0));
}

void Transform::Scale(glm::mat4& Matrix, GLfloat X, GLfloat Y) {
	Matrix = scale(Matrix, glm::vec3(X, Y, 1.0));
}

void Transform::Flip(glm::mat4& Matrix, FlipDir FlipOption) {
	if (FlipOption != static_cast<FlipDir>(-1)) {
		switch (FlipOption) {
		case FlipDir::H:
			Matrix = rotate(Matrix, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
			break;

		case FlipDir::V:
			Matrix = rotate(Matrix, glm::radians(180.0f), glm::vec3(1.0, 0.0, 0.0));
			break;

		case FlipDir::HV:
			Matrix = rotate(Matrix, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
			Matrix = rotate(Matrix, glm::radians(180.0f), glm::vec3(1.0, 0.0, 0.0));
			break;
		}
	}
}
