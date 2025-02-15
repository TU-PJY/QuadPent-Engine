#pragma once
#include "SDKHeader.h"
#include "ObjectValue.h"

class TransformUtil {
public:
	void Identity(glm::mat4& Matrix);
	void Move(glm::mat4& Matrix, GLfloat X, GLfloat Y);
	void Move(glm::mat4& Matrix, glm::vec2& Position);
	void Rotate(glm::mat4& Matrix, GLfloat Degree);
	void RotateRadians(glm::mat4& Matrix, GLfloat Radians);
	void Scale(glm::mat4& Matrix, GLfloat X, GLfloat Y);
	void Scale(glm::mat4& Matrix, glm::vec2& Size);
	void Tilt(glm::mat4& Matrix, GLfloat X, GLfloat Y);
	void Tilt(glm::mat4& Matrix, glm::vec2& Value);
	void ImageScale(glm::mat4& Matrix, GLfloat Width, GLfloat Height);
	void Flip(int FlipFlag);
	void UnitFlip(int FlipFlag);
	bool CheckIdentity(glm::mat4& Matrix);
	void RotateV(glm::mat4& Matrix, GLfloat Degree);
	void RotateH(glm::mat4& Matrix, GLfloat Degree);
};

namespace SDK {
	extern TransformUtil Transform;
}