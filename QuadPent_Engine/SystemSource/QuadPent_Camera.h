#pragma once
#include "QuadPent_Header.h"

namespace QP {
	class QuadPent_Camera {
	private:
		QP::Vector3 CamPos{}, CamDirection{}, CamUp{};
		bool      StaticRenderCommand{};
	
	public:
		QP::Matrix4 ViewMatrix{ 1.0 }, Projection{ 1.0 }, CameraMatrix{ 1.0 };

		QP::Vector2 Position{};
		float   Rotation{};
		float   Zoom{ 1.0f };

		void CalculateASPECT();
		void Init();
		void SetCamera(int RenderType);
		void PrepareRender(int ShaderType);
	};

	extern QuadPent_Camera Camera;
}