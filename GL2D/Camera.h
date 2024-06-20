#pragma once
#include "GLHeader.h"

enum class Zoom{ In, Out };

class Camera {
protected:
	glm::vec3 CamPos{}, CamDirection{}, CamUp{};
	glm::mat4 ViewMatrix{}, Projection{};
	unsigned int ProjectionLocation{}, ViewLocation{}, ViewPosLocation{};
public:

	GLfloat Rotation{};
	GLfloat x{}, y{};
	GLfloat ZoomValue = 1.0f;

	bool UseTextShader{};

	void CalculateASPECT() {
		ASPECT = 1.0 * WIDTH / HEIGHT;

		rect.lx = -1.0 * ASPECT;
		rect.ly = -1.0;
		rect.rx = 1.0 * ASPECT;
		rect.ry = 1.0;
	}

	void SetCamera() {
		using namespace glm;

		ViewMatrix = mat4(1.0f);
		Projection = mat4(1.0f);

		CamPos = vec3(0.0f, 0.0f, 1.0f);
		CamDirection = vec3(0.0f, 0.0f, 0.0f);
		CamUp = vec3(0.0f, 1.0f, 0.0f);

		ViewMatrix = lookAt(CamPos, CamDirection, CamUp);

		ViewMatrix = translate(ViewMatrix, vec3(x, y, 0.0));
		ViewMatrix = rotate(ViewMatrix, radians(Rotation), vec3(0.0, 0.0, 1.0));

		Projection = ortho((ASPECT * -1.0f) / ZoomValue, (ASPECT * 1.0f) / ZoomValue, -1.0f / ZoomValue, 1.0f / ZoomValue, -100.0f, 100.0f);
	
		ProcessTransform();
	}

	void ProcessTransform() {
		if (UseTextShader) {
			ProjectionLocation = glGetUniformLocation(TextShader, "projection");
			glUniformMatrix4fv(ProjectionLocation, 1, GL_FALSE, &Projection[0][0]);

			ViewLocation = glGetUniformLocation(TextShader, "view");
			glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, &ViewMatrix[0][0]);

			ViewPosLocation = glGetUniformLocation(TextShader, "viewPos");
			glUniform3f(ViewPosLocation, CamPos.x, CamPos.y, CamPos.z);

			UseTextShader = false;
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
};
extern Camera cam;

class CamaraControlUtil : public Camera {
public:
	void Move(GLfloat MoveX, GLfloat MoveY) {
		x = MoveX;
		y = MoveY;
	}

	void MoveAspect(GLfloat MoveX, GLfloat MoveY) {
		x = MoveX * ASPECT;
		y = MoveY;
	}

	void Rotate(GLfloat Radians) {
		Rotation = Radians;
	}

	void Zoom(GLfloat Value, Zoom ZoomOpt) {
		GLfloat UpdatedZoomValue{};

		switch (ZoomOpt) {
		case Zoom::In:
			UpdatedZoomValue = ZoomValue / (1.0f - Value);
			break;

		case Zoom::Out:
			UpdatedZoomValue = ZoomValue * (1.0f - Value);
			break;
		}

		ZoomValue = UpdatedZoomValue;
	}
};
extern CamaraControlUtil camUtil;