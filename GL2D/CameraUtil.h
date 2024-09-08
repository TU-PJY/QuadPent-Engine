#pragma once
#include "EngineHeader.h"

enum class ZoomType{ In, Out };

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
	void SetCamera(RenderType Type);
	void ProcessTransform(ShaderType Type);
	void Zoom(ZoomType Type, GLfloat Value);
	void ChangeZoom(GLfloat Value);
	void InitMatrix();
};
extern Camera camera;

GLfloat DivZoom(GLfloat Value);