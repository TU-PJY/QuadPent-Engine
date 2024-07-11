#include "CameraUtil.h"

void CameraUtil::CalculateASPECT() {
	ASPECT = 1.0 * WIDTH / HEIGHT;
	rect.lx = -1.0 * ASPECT;
	rect.ly = -1.0;
	rect.rx = 1.0 * ASPECT;
	rect.ry = 1.0;
}

void CameraUtil::Init() {
	CalculateASPECT();
	SetCamera();
}

void CameraUtil::SetCamera() {
	using namespace glm;
	CalculateASPECT();

	ViewMatrix = mat4(1.0f);
	Projection = mat4(1.0f);

	CamPos = vec3(0.0f, 0.0f, 1.0f);
	CamDirection = vec3(0.0f, 0.0f, 0.0f);
	CamUp = vec3(0.0f, 1.0f, 0.0f);

	ViewMatrix = lookAt(CamPos, CamDirection, CamUp);

	ViewMatrix = translate(ViewMatrix, glm::vec3(x, y, 0.0));
	ViewMatrix = rotate(ViewMatrix, glm::radians(Rotation), glm::vec3(0.0, 0.0, 1.0));

	Projection = ortho((ASPECT * -1.0f) / Zoom, (ASPECT * 1.0f) / Zoom, -1.0f / Zoom, 1.0f / Zoom, -100.0f, 100.0f);
}

void CameraUtil::ProcessTransform(bool UseTextShader) {
	if (UseTextShader) {
		ProjectionLocation = glGetUniformLocation(TextShader, "projection");
		glUniformMatrix4fv(ProjectionLocation, 1, GL_FALSE, &Projection[0][0]);

		ViewLocation = glGetUniformLocation(TextShader, "view");
		glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, &ViewMatrix[0][0]);

		ViewPosLocation = glGetUniformLocation(TextShader, "viewPos");
		glUniform3f(ViewPosLocation, CamPos.x, CamPos.y, CamPos.z);
	}

	else {
		ProjectionLocation = glGetUniformLocation(ImageShader, "projection");
		glUniformMatrix4fv(ProjectionLocation, 1, GL_FALSE, &Projection[0][0]);

		ViewLocation = glGetUniformLocation(ImageShader, "view");
		glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, &ViewMatrix[0][0]);

		ViewPosLocation = glGetUniformLocation(ImageShader, "viewPos");
		glUniform3f(ViewPosLocation, CamPos.x, CamPos.y, CamPos.z);
	}
}

// Divide value with camera zoom value
GLfloat DivZoom(GLfloat Value) {
	return Value / camera.Zoom;
}

GLfloat SubRot(GLfloat Radians) {
	return Radians - camera.Rotation;
}

GLfloat SubPosX(GLfloat X) {
	return X - camera.x;
}

GLfloat SubPosY(GLfloat Y) {
	return Y - camera.y;
}



void CamaraControlUtil::Translate(GLfloat X, GLfloat Y) {
	camera.x = X; 
	camera.y = Y;
}

void CamaraControlUtil::Rotate(GLfloat Radians) {
	camera.Rotation = Radians;
}

void CamaraControlUtil::SetZoom(ZOOM ZoomOpt, GLfloat Value) {
	GLfloat UpdatedZoomValue{};

	switch (ZoomOpt) {
	case ZOOM::In:
		UpdatedZoomValue = camera.Zoom / (1.0f - Value);
		camera.Zoom = UpdatedZoomValue;
		break;

	case ZOOM::Out:
		UpdatedZoomValue = camera.Zoom * (1.0f - Value);
		camera.Zoom = UpdatedZoomValue;
		break;
	}

	camera.Zoom = UpdatedZoomValue;
}