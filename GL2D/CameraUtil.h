#pragma once
#include "EngineHeader.h"

enum class ZoomOpt{ In, Out };

class Camera {
private:
	glm::vec3 CamPos{}, CamDirection{}, CamUp{};
	unsigned int ProjectionLocation{}, ViewLocation{}, ViewPosLocation{};
	bool StaticMode{};
	
public:
	glm::mat4 ViewMatrix{}, Projection{};
	GLfloat ZoomValue{ 1.0f };

	void CalculateASPECT();
	void Init();
	void SetCamera(bool Flag=false);
	void ProcessTransform(bool UseTextShader);
	void Move(GLfloat X, GLfloat Y);
	void Rotation(GLfloat Rotation);
	void Zoom(ZoomOpt Type, GLfloat Value);
	void ChangeZoom(GLfloat Value);
};
extern Camera camera;

GLfloat DivZoom(GLfloat Value);