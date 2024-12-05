#pragma once
#include "EngineHeader.h"

namespace Transform {
	void Identity(glm::mat4& Matrix);
	void Move(glm::mat4& Matrix, GLfloat X, GLfloat Y);
	void Move(glm::mat4& Matrix, glm::vec2& Position);
	void Rotate(glm::mat4& Matrix, GLfloat Degree);
	void RotateRad(glm::mat4& Matrix, GLfloat Radians);
	void Scale(glm::mat4& Matrix, GLfloat X, GLfloat Y);
	void Scale(glm::mat4& Matrix, glm::vec2& Size);
	void ImageScale(glm::mat4& Matrix, GLfloat Width, GLfloat Height);
	void RotateY(glm::mat4& Matrix, GLfloat Degree);
	void RotateX(glm::mat4& Matrix, GLfloat Degree);
}