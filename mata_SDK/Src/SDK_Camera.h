#pragma once
#include "SDK_Header.h"

class SDK_Camera {
private:
	glm::vec3 CamPos{}, CamDirection{}, CamUp{};
	bool      StaticRenderCommand{};
	
public:
	glm::mat4 ViewMatrix{ 1.0 }, Projection{ 1.0 }, CameraMatrix{ 1.0 };

	glm::vec2 Position{};
	GLfloat   Rotation{};
	GLfloat   Zoom{ 1.0f };

	void CalculateASPECT();
	void Init();
	void SetCamera(int RenderType);
	void PrepareRender(int ShaderType);
};

namespace SDK {
	extern SDK_Camera Camera;
}