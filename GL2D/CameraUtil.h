#pragma once
#include "GLHeader.h"

enum class ZOOM{ In, Out };

class CameraUtil {
protected:
	glm::vec3 CamPos{}, CamDirection{}, CamUp{};
	glm::mat4 ViewMatrix{}, Projection{};
	unsigned int ProjectionLocation{}, ViewLocation{}, ViewPosLocation{};
	
public:
	GLfloat Rotation{};
	GLfloat x{}, y{};
	GLfloat Zoom{ 1.0f };

	void CalculateASPECT();
	void SetCamera();
	void ProcessTransform(bool UseTextShader);
};
extern CameraUtil cam;


class CamaraControlUtil : public CameraUtil {
public:
	void Move(GLfloat MoveX, GLfloat MoveY);
	void Rotate(GLfloat Radians);
	void ZoomCamera(GLfloat Value, ZOOM ZoomOpt);
};
extern CamaraControlUtil camUtil;