#pragma once
#include "EngineHeader.h"

class LineBrush {
private:
	unsigned int ModelLocation{}, TransparencyLocation{}, ObjectColorLocation{};
	glm::mat4 TranslateMatrix{ 1.0f }, ScaleMatrix{ 1.0f };

	GLfloat Width{};
	GLfloat Rotation;
	GLfloat Length{};
	glm::vec3 Color{};

	RenderType Type { RenderType::Default };

public:
	void SetRenderType(RenderType Opt);
	void SetColor(GLfloat R, GLfloat G, GLfloat B);
	void SetColorRGB(int R, int G, int B);
	void Draw(GLfloat X1, GLfloat Y1, GLfloat X2, GLfloat Y2, GLfloat Width, GLfloat Transparency = 1.0);
	void DrawStraightX(GLfloat X1, GLfloat X2, GLfloat Width, GLfloat Transparency = 1.0);
	void DrawStraightY(GLfloat Y1, GLfloat Y2, GLfloat Width, GLfloat Transparency = 1.0);
};