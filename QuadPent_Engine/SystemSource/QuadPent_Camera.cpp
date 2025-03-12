#include "QuadPent_Camera.h"
#include "QuadPent_Transform.h"

void QP::QuadPent_Camera::CalculateASPECT() {
	QP::Aspect = (float)QP::WindowWidth / (float)QP::WindowHeight;
	QP::ViewportWidth = 2.0 * QP::Aspect;
	QP::ViewportHeight = 2.0;
	QP::WindowRect.LeftX = -1.0 * QP::Aspect;
	QP::WindowRect.LeftY = -1.0;
	QP::WindowRect.RightX = 1.0 * QP::Aspect;
	QP::WindowRect.RightY = 1.0;
	QP::PrevWindowWidth = QP::WindowWidth;
	QP::PrevWindowHeight = QP::WindowHeight;
}

void QP::QuadPent_Camera::Init() {
	CalculateASPECT();

	QP::Transform.Identity(ViewMatrix);
	QP::Transform.Identity(Projection);
	QP::Transform.Identity(CameraMatrix);

	SetCamera(RENDER_TYPE_DEFAULT);
}

void QP::QuadPent_Camera::SetCamera(int RenderType) {
	if(QP::PrevWindowWidth != QP::WindowWidth || QP::PrevWindowHeight != QP::WindowHeight)
		CalculateASPECT();

	CamPos = QP::Vector3(0.0f, 0.0f, 1.0f);
	CamDirection = QP::Vector3(0.0f, 0.0f, -1.0f);
	CamUp = QP::Vector3(0.0f, 1.0f, 0.0f);

	StaticRenderCommand = RenderType;
}

void QP::QuadPent_Camera::PrepareRender(int ShaderType) {
	QP::Transform.Identity(ViewMatrix);
	QP::Transform.Identity(Projection);

	if (!StaticRenderCommand) {
		ViewMatrix = lookAt(CamPos, CamDirection, CamUp);
		ViewMatrix = ViewMatrix * CameraMatrix;
		Projection = glm::ortho((QP::Aspect * -1.0f) / Zoom, (QP::Aspect * 1.0f) / Zoom, -1.0f / Zoom, 1.0f / Zoom, -10.0f, 10.0f);
	}
	else {
		ViewMatrix = lookAt(CamPos, CamDirection, CamUp);
		Projection = glm::ortho((QP::Aspect * -1.0f), (QP::Aspect * 1.0f), -1.0f, 1.0f, -10.0f, 10.0f);
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
