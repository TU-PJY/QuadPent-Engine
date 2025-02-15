#pragma once
#include "CameraUtil.h"

class SDK_CameraController {
public:
	void Update(float FrameTime);
	void Move(GLfloat X, GLfloat Y);
	void Move(glm::vec2& PositionValue);
	void Rotate(GLfloat Degree);
	void Zoom(int ZoomType, GLfloat ZoomValue);
	void SetZoom(GLfloat ZoomValue);
	GLfloat ComputeNextZoom(int ZoomType, GLfloat ZoomValue);

private:
	glm::vec2 Position{};
	GLfloat Rotation{};

	void ComputeCameraMatrix();
};

namespace SDK {
	extern SDK_CameraController CameraControl;
	extern glm::vec2 CameraPosition;
	extern GLfloat CameraRotation;
	extern GLfloat CameraZoom;
}