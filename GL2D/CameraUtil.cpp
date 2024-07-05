#include "CameraUtil.h"

void CameraUtil::CalculateASPECT() {
	ASPECT = 1.0 * WIDTH / HEIGHT;

	rect.lx = -1.0 * ASPECT;
	rect.ly = -1.0;
	rect.rx = 1.0 * ASPECT;
	rect.ry = 1.0;
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
void DivZoom(GLfloat& Value) {
	Value /= cam.Zoom;
}

void SubRot(GLfloat& Value) {
	Value -= cam.Rotation;
}

void SubPos(GLfloat& X, GLfloat& Y) {
	X = X - cam.x;
	Y = Y - cam.y;
}


void CamaraControlUtil::Translate(GLfloat X, GLfloat Y) {
	cam.x = X; 
	cam.y = Y;
}

void CamaraControlUtil::Rotate(GLfloat Radians) {
	cam.Rotation = Radians;
}

void CamaraControlUtil::SetZoom(ZOOM ZoomOpt, GLfloat Value) {
	GLfloat UpdatedZoomValue{};

	switch (ZoomOpt) {
	case ZOOM::In:
		UpdatedZoomValue = cam.Zoom / (1.0f - Value);
		cam.Zoom = UpdatedZoomValue;
		break;

	case ZOOM::Out:
		UpdatedZoomValue = cam.Zoom * (1.0f - Value);
		cam.Zoom = UpdatedZoomValue;
		break;
	}

	cam.Zoom = UpdatedZoomValue;
}