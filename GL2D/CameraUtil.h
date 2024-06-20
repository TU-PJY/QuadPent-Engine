#pragma once
#include "GLHeader.h"

enum class Zoom{ In, Out };

class CameraUtil {
protected:
	glm::vec3 CamPos{}, CamDirection{}, CamUp{};
	glm::mat4 ViewMatrix{}, Projection{};
	unsigned int ProjectionLocation{}, ViewLocation{}, ViewPosLocation{};
public:

	GLfloat Rotation{};
	GLfloat x{}, y{};
	GLfloat ZoomValue = 1.0f;

	bool UseTextShader{};

	void CalculateASPECT();
	void SetCamera();
	void ProcessTransform();
};
extern CameraUtil cam;


class CamaraControlUtil : public CameraUtil {
public:
	void Move(GLfloat MoveX, GLfloat MoveY);
	void MoveAspect(GLfloat MoveX, GLfloat MoveY);
	void Rotate(GLfloat Radians);
	void ZoomCamera(GLfloat Value, Zoom ZoomOpt);
};
extern CamaraControlUtil camUtil;