#pragma once
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <string>

class OBJ {
public:
	bool DeleteFlag{};
	bool StopAtPauseFlag{ true };
	std::string ObjectTag{};

	glm::mat4 TranslateMatrix{ 1.0f }, RotateMatrix{ 1.0f }, ScaleMatrix{ 1.0f };
	GLfloat AlphaValue{ 1.0f };
	unsigned int ModelLocation{}, TransparencyLocation{}, ObjectColorLocation{}, TextAlphaLocation{};

	void Move(GLfloat MoveX, GLfloat MoveY);
	void Rotate(GLfloat Radians);
	void Scale(GLfloat ScaleX, GLfloat ScaleY);
	void RotateSpot(GLfloat Radians);
	void MoveAspect(GLfloat MoveX, GLfloat MoveY);
	void SetAlpha(GLfloat Value);
	void InitTransform();
	void ProcessTransform();

	virtual void Update(float FT) {}
	virtual void Render() {}
};