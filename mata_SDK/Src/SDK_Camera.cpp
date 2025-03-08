#include "SDK_Camera.h"
#include "SDK_Transform.h"

void SDK::SDK_Camera::CalculateASPECT() {
	SDK::Aspect = (float)SDK::WindowWidth / (float)SDK::WindowHeight;
	SDK::ViewportWidth = 2.0 * SDK::Aspect;
	SDK::ViewportHeight = 2.0;
	SDK::WindowRect.LeftX = -1.0 * SDK::Aspect;
	SDK::WindowRect.LeftY = -1.0;
	SDK::WindowRect.RightX = 1.0 * SDK::Aspect;
	SDK::WindowRect.RightY = 1.0;

	SDK::PrevWindowWidth = SDK::WindowWidth;
	SDK::PrevWindowHeight = SDK::WindowHeight;
}

void SDK::SDK_Camera::Init() {
	CalculateASPECT();

	SDK::Transform.Identity(ViewMatrix);
	SDK::Transform.Identity(Projection);
	SDK::Transform.Identity(CameraMatrix);

	SetCamera(RENDER_TYPE_DEFAULT);
}

void SDK::SDK_Camera::SetCamera(int RenderType) {
	if(SDK::PrevWindowWidth != SDK::WindowWidth || SDK::PrevWindowHeight != SDK::WindowHeight)
		CalculateASPECT();

	CamPos = SDK::Vector3(0.0f, 0.0f, 1.0f);
	CamDirection = SDK::Vector3(0.0f, 0.0f, -1.0f);
	CamUp = SDK::Vector3(0.0f, 1.0f, 0.0f);

	StaticRenderCommand = RenderType;
}

void SDK::SDK_Camera::PrepareRender(int ShaderType) {
	SDK::Transform.Identity(ViewMatrix);
	SDK::Transform.Identity(Projection);

	if (!StaticRenderCommand) {
		ViewMatrix = lookAt(CamPos, CamDirection, CamUp);
		ViewMatrix = ViewMatrix * CameraMatrix;
		Projection = glm::ortho((SDK::Aspect * -1.0f) / Zoom, (SDK::Aspect * 1.0f) / Zoom, -1.0f / Zoom, 1.0f / Zoom, -10.0f, 10.0f);
	}
	else {
		ViewMatrix = lookAt(CamPos, CamDirection, CamUp);
		Projection = glm::ortho((SDK::Aspect * -1.0f), (SDK::Aspect * 1.0f), -1.0f, 1.0f, -10.0f, 10.0f);
	}

	switch (ShaderType) {
	case SHADER_TYPE_IMAGE:
		glUniformMatrix4fv(IMAGE_PROJECTION_LOCARION, 1, GL_FALSE, &Projection[0][0]);
		glUniformMatrix4fv(IMAGE_VIEW_LOCATION, 1, GL_FALSE, &ViewMatrix[0][0]);
		glUniform3f(IMAGE_VIEW_POSITION_LOCATION, CamPos.x, CamPos.y, CamPos.z);
		break;

	case SHADER_TYPE_TEXT:
		glUniformMatrix4fv(TEXT_PROJECTION_LOCATION, 1, GL_FALSE, &Projection[0][0]);
		glUniformMatrix4fv(TEXT_VIEW_LOCATION, 1, GL_FALSE, &ViewMatrix[0][0]);
		glUniform3f(TEXT_VIEW_POSITION_LOCATION, CamPos.x, CamPos.y, CamPos.z);
		break;

	case SHADER_TYPE_SHAPE:
		glUniformMatrix4fv(SHAPE_PROJECTION_LOCATION, 1, GL_FALSE, &Projection[0][0]);
		glUniformMatrix4fv(SHAPE_VIEW_LOCATION, 1, GL_FALSE, &ViewMatrix[0][0]);
		glUniform3f(SHAPE_VIEW_POSITION_LOCATION, CamPos.x, CamPos.y, CamPos.z);
		break;
	}
}
