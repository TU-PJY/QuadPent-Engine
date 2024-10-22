#pragma once
#include "EngineHeader.h"

enum FlipDir
{ FLIP_H, FLIP_V, FLIP_HV };

namespace Transform {
	void Move(glm::mat4& Matrix, GLfloat X, GLfloat Y);
	void Rotate(glm::mat4& Matrix, GLfloat Degree);
	void RotateRad(glm::mat4& Matrix, GLfloat Radians);
	void Scale(glm::mat4& Matrix, GLfloat X, GLfloat Y);
	void RotateV(glm::mat4& Matrix, GLfloat Degree);
	void RotateH(glm::mat4& Matrix, GLfloat Degree);
	void Flip(glm::mat4& Matrix, int FlipOption);
}