#pragma once
#include "EngineHeader.h"

enum class ZoomOpt{ In, Out };

class Camera {
private:
	glm::vec3 CamPos{}, CamDirection{}, CamUp{};
	unsigned int ProjectionLocation{}, ViewLocation{}, ViewPosLocation{};
	bool StaticMode{};
	
public:
	glm::mat4 ViewMatrix{}, Projection{}, TranslateMatrix{}, RotateMatrix{};
	GLfloat ZoomValue{ 1.0f };

	void CalculateASPECT();
	void Init();
	void SetCamera(bool Flag=false);
	void ProcessTransform(bool UseTextShader);
	void Zoom(ZoomOpt Type, GLfloat Value);
	void ChangeZoom(GLfloat Value);
	void InitMatrix();
};
extern Camera camera;

GLfloat DivZoom(GLfloat Value);