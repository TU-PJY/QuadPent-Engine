#pragma once
#include "MATA_HEADER.h"

enum class ZOOM{ In, Out };

class CameraUtil {
private:
	glm::vec3 CamPos{}, CamDirection{}, CamUp{};
	unsigned int ProjectionLocation{}, ViewLocation{}, ViewPosLocation{};
	
public:
	glm::mat4 ViewMatrix{}, Projection{};
	GLfloat Rotation{};
	GLfloat x{}, y{};
	GLfloat Zoom{ 1.0f };

	void CalculateASPECT();
	void Init();
	void SetCamera();
	void SetStaticCamera();
	void ProcessTransform(bool UseTextShader);
};
extern CameraUtil camera;


class CamaraControlUtil {
public:
	void Move(GLfloat MoveX, GLfloat MoveY);
	void Rotate(GLfloat Radians);
	void AdjustZoom(ZOOM ZoomOpt, GLfloat Value);
};
extern CamaraControlUtil camUtil;

GLfloat DivZoom(GLfloat Value);