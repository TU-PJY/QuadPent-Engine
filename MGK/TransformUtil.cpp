#include "TransformUtil.h"

void Transform::Identity(glm::mat4& Matrix) {
	Matrix = glm::mat4(1.0f);
}

void Transform::Move(glm::mat4& Matrix, GLfloat X, GLfloat Y) {
	Matrix = translate(Matrix, glm::vec3(X, Y, 0.0));
}

void Transform::Move(glm::mat4& Matrix, glm::vec2& Position) {
	Matrix = translate(Matrix, glm::vec3(Position.x, Position.y, 0.0));
}

void Transform::Rotate(glm::mat4& Matrix, GLfloat Degree) {
	Matrix = rotate(Matrix, glm::radians(Degree), glm::vec3(0.0, 0.0, 1.0));
}

void Transform::RotateRad(glm::mat4& Matrix, GLfloat Radians) {
	Matrix = rotate(Matrix, Radians, glm::vec3(0.0, 0.0, 1.0));
}

void Transform::RotateY(glm::mat4& Matrix, GLfloat Degree) {
	Matrix = rotate(Matrix, glm::radians(Degree), glm::vec3(1.0, 0.0, 0.0));
}

void Transform::RotateX(glm::mat4& Matrix, GLfloat Degree) {
	Matrix = rotate(Matrix, glm::radians(Degree), glm::vec3(0.0, 1.0, 0.0));
}

void Transform::Scale(glm::mat4& Matrix, GLfloat X, GLfloat Y) {
	Matrix = scale(Matrix, glm::vec3(X, Y, 1.0));
}

void Transform::Scale(glm::mat4& Matrix, glm::vec2& Size) {
	Matrix = scale(Matrix, glm::vec3(Size.x, Size.y, 1.0));
}

void Transform::ImageScale(glm::mat4& Matrix, GLfloat Width, GLfloat Height) {
	if (Width > Height)
		Matrix = glm::scale(Matrix, glm::vec3(1.0, (GLfloat)Height / (GLfloat)Width, 1.0));
	else if (Width < Height)
		Matrix = glm::scale(Matrix, glm::vec3((GLfloat)Width / (GLfloat)Height, 1.0, 1.0));
}