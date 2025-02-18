#pragma once
#include "SDK_Camera.h"

namespace SDK {
	class SDK_CameraController {
	public:
		void Update(float FrameTime);
		void Move(float X, float Y);
		void Move(glm::vec2& PositionValue);
		void Rotate(float Degree);
		void Zoom(int ZoomType, float ZoomValue);
		void SetZoom(float ZoomValue);
		float ComputeNextZoom(int ZoomType, float ZoomValue);

	private:
		glm::vec2 Position{};
		float Rotation{};

		void ComputeCameraMatrix();
	};

	extern SDK::SDK_CameraController CameraControl;
}