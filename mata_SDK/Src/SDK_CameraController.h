#pragma once
#include "SDK_Camera.h"

namespace SDK {
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

	extern SDK::SDK_CameraController CameraControl;
}