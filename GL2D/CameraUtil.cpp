#include "CameraUtil.h"

void Camera::CalculateASPECT() {
	ASPECT = 1.0 * WIDTH / HEIGHT;
	rect.lx = -1.0 * ASPECT;
	rect.ly = -1.0;
	rect.rx = 1.0 * ASPECT;
	rect.ry = 1.0;

	PREV_WIDTH = WIDTH;
	PREV_HEIGHT = HEIGHT;
}

void Camera::Init() {
	CalculateASPECT();
	InitMatrix();
	SetCamera(RenderType::Default);
}

void Camera::SetCamera(RenderType Type) {
	using namespace glm;

	if(PREV_WIDTH != WIDTH || PREV_HEIGHT != HEIGHT)
		CalculateASPECT();

	CamPos = vec3(0.0f, 0.0f, 1.0f);
	CamDirection = vec3(0.0f, 0.0f, 0.0f);
	CamUp = vec3(0.0f, 1.0f, 0.0f);

	if (Type == RenderType::Default)
		StaticMode = false;
	else if (Type == RenderType::Static)
		StaticMode = true;
}

void Camera::ProcessTransform(ShaderType Type) {
	if (StaticMode) {
		ViewMatrix = glm::mat4(1.0f);
		Projection = glm::mat4(1.0f);
		ViewMatrix = lookAt(CamPos, CamDirection, CamUp);
		Projection = glm::ortho((ASPECT * -1.0f), (ASPECT * 1.0f), -1.0f, 1.0f, -100.0f, 100.0f);
	}

	else {
		ViewMatrix = glm::mat4(1.0f);
		Projection = glm::mat4(1.0f);
		ViewMatrix = lookAt(CamPos, CamDirection, CamUp);
		ViewMatrix = ViewMatrix * TranslateMatrix * RotateMatrix;
		Projection = glm::ortho((ASPECT * -1.0f) / ZoomValue, (ASPECT * 1.0f) / ZoomValue, -1.0f / ZoomValue, 1.0f / ZoomValue, -100.0f, 100.0f);
	}

	if (Type == ShaderType::Image) {
		ProjectionLocation = glGetUniformLocation(ImageShader, "projection");
		glUniformMatrix4fv(ProjectionLocation, 1, GL_FALSE, &Projection[0][0]);

		ViewLocation = glGetUniformLocation(ImageShader, "view");
		glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, &ViewMatrix[0][0]);

		ViewPosLocation = glGetUniformLocation(ImageShader, "viewPos");
		glUniform3f(ViewPosLocation, CamPos.x, CamPos.y, CamPos.z);
	}

	else if (Type == ShaderType::Text) {
		ProjectionLocation = glGetUniformLocation(TextShader, "projection");
		glUniformMatrix4fv(ProjectionLocation, 1, GL_FALSE, &Projection[0][0]);

		ViewLocation = glGetUniformLocation(TextShader, "view");
		glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, &ViewMatrix[0][0]);

		ViewPosLocation = glGetUniformLocation(TextShader, "viewPos");
		glUniform3f(ViewPosLocation, CamPos.x, CamPos.y, CamPos.z);
	}
}

void Camera::InitMatrix() {
	TranslateMatrix = glm::mat4(1.0f);
	RotateMatrix = glm::mat4(1.0f);
}

void Camera::Zoom(ZoomType Type, GLfloat Value) {
	if(Type == ZoomType::In)
		ZoomValue = ZoomValue / (1.0f - Value);

	else if(Type == ZoomType::Out) 
		ZoomValue = ZoomValue * (1.0f - Value);
}

void Camera::ChangeZoom(GLfloat Value) {
	ZoomValue = Value;
}

GLfloat DivZoom(GLfloat Value) {
	return Value / camera.ZoomValue;
}