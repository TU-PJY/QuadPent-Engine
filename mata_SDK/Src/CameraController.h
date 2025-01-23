#pragma once
#include "GameObject.h"

class CameraController : public GameObject {
public:
	glm::vec2 Position{};
	GLfloat Rotation{};

	void InputKey(KeyEvent& Event);
	void UpdateFunc(float FT);
	void MoveCamera(GLfloat X, GLfloat Y);
	void MoveCamera(glm::vec2& PositionValue);
	void RotateCamera(GLfloat Degree);
	void CameraZoom(int ZoomType, GLfloat ZoomValue);
	void ChangeCameraZoom(GLfloat ZoomValue);
	GLfloat ComputeNextZoom(int ZoomType, GLfloat ZoomValue);

private:
	void ComputeCameraMatrix();
};

extern GameObject* CameraControl;