#include "SDK_Camera.h"
#include "SDK_Transform.h"

void SDK_Camera::CalculateASPECT() {
	MSDK::ASPECT = (float)MSDK::WIDTH / (float)MSDK::HEIGHT;
	MSDK::WindowRect.LeftX = -1.0 * MSDK::ASPECT;
	MSDK::WindowRect.LeftY = -1.0;
	MSDK::WindowRect.RightX = 1.0 * MSDK::ASPECT;
	MSDK::WindowRect.RightY = 1.0;

	MSDK::PREV_WIDTH = MSDK::WIDTH;
	MSDK::PREV_HEIGHT = MSDK::HEIGHT;
}

void SDK_Camera::Init() {
	CalculateASPECT();

	MSDK::Transform.Identity(ViewMatrix);
	MSDK::Transform.Identity(Projection);
	MSDK::Transform.Identity(CameraMatrix);

	SetCamera(RENDER_TYPE_DEFAULT);
}

void SDK_Camera::SetCamera(int RenderType) {
	using namespace glm;

	if(MSDK::PREV_WIDTH != MSDK::WIDTH || MSDK::PREV_HEIGHT != MSDK::HEIGHT)
		CalculateASPECT();

	CamPos = vec3(0.0f, 0.0f, 1.0f);
	CamDirection = vec3(0.0f, 0.0f, 0.0f);
	CamUp = vec3(0.0f, 1.0f, 0.0f);

	StaticRenderCommand = RenderType;
}

void SDK_Camera::PrepareRender(int ShaderType) {
	MSDK::Transform.Identity(ViewMatrix);
	MSDK::Transform.Identity(Projection);

	if (!StaticRenderCommand) {
		ViewMatrix = lookAt(CamPos, CamDirection, CamUp);
		ViewMatrix = ViewMatrix * CameraMatrix;
		Projection = glm::ortho((MSDK::ASPECT * -1.0f) / Zoom, (MSDK::ASPECT * 1.0f) / Zoom, -1.0f / Zoom, 1.0f / Zoom, -10.0f, 10.0f);
	}
	else {
		ViewMatrix = lookAt(CamPos, CamDirection, CamUp);
		Projection = glm::ortho((MSDK::ASPECT * -1.0f), (MSDK::ASPECT * 1.0f), -1.0f, 1.0f, -10.0f, 10.0f);
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
