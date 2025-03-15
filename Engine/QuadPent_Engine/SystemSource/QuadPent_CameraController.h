#pragma once
#include "QuadPent_Camera.h"

namespace QP {
	class QuadPent_CameraController {
	public:
		void Update();
		void Move(float X, float Y);
		void Move(QP::Vector2& PositionValue);
		void Rotate(float Degree);
		void Zoom(int ZoomType, float ZoomValue);
		void SetZoom(float ZoomValue);
		float ComputeNextZoom(int ZoomType, float ZoomValue);

	private:
		QP::Vector2 Position{};
		float Rotation{};
		float ControllerZoom{ 1.0 };

		void ComputeCameraMatrix();
	};

	extern QP::QuadPent_CameraController CameraControl;
}