#pragma once
#include "EngineHeader.h"

enum class FlipDir
{ H, V };

namespace Transform {
	void Move(glm::mat4& Matrix, GLfloat X, GLfloat Y);
	void Rotate(glm::mat4& Matrix, GLfloat Degree);
	void Scale(glm::mat4& Matrix, GLfloat X, GLfloat Y);
	void RotateV(glm::mat4& Matrix, GLfloat Degree);
	void RotateH(glm::mat4& Matrix, GLfloat Degree);
	void Flip(glm::mat4& Matrix, FlipDir FlipOption);
}