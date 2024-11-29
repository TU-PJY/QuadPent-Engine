#pragma once
#include "EngineHeader.h"

class CircleBrush {
private:
	glm::mat4 TranslateMatrix{ 1.0f };

	glm::vec3 Color{};
	GLfloat   Radius{};
	GLfloat   TransparencyValue{};
	int       RenderType{ RENDER_TYPE_DEFAULT };

	bool      CamInheritanceCommand{};

public:
	CircleBrush(bool Flag = false);
	void SetColor(GLfloat R, GLfloat G, GLfloat B);
	void SetColorRGB(int R, int G, int B);
	void SetRenderType(int Opt);
	void Draw(GLfloat X, GLfloat Y, GLfloat Diameter, GLfloat Transparency=1.0f);

private:
	void Render();
};

class LineCircleBrush {
private:
	glm::mat4 TranslateMatrix{ 1.0f };

	glm::vec3 Color{};
	GLfloat   Radius{}, WidthValue{};
	GLfloat   TransparencyValue{};
	int       RenderType{ RENDER_TYPE_DEFAULT };

	bool      CamInheritanceCommand{};

public:
	LineCircleBrush(bool Flag = false);
	void SetColor(GLfloat R, GLfloat G, GLfloat B);
	void SetColorRGB(int R, int G, int B);
	void SetRenderType(int Opt);
	void Draw(GLfloat X, GLfloat Y, GLfloat Diameter, GLfloat Width, GLfloat Transparency = 1.0f);

private:
	void Render();
};