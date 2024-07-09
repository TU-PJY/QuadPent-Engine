#pragma once
#include "MATA_HEADER.h"
#include "Collisionutil.h"

enum class ButtonFlip
{ Horizontal, Vertical };

enum class CursorFlip
{ Horizontal, Vertical };


class Button {
private:
	unsigned int ModelLocation{}, TransparencyLocation{}, ObjectColorLocation{}, TextAlphaLocation{};
	unsigned int Image{};
	glm::mat4 TranslateMatrix{ 1.0f };
	glm::vec3 ButtonColor{ 1.0f, 1.0f, 1.0f };
	GLfloat Transparency{ 1.0f };

	GLfloat PositionX{}, PositionY{};
	GLfloat SizeX{}, SizeY{};

	AABB aabb;

public:
	void Init(std::string ImageName);
	void Move(GLfloat X, GLfloat Y);
	void Rotate(GLfloat RotationValue);
	void Scale(GLfloat X, GLfloat Y);
	void SetColor(GLfloat R, GLfloat G, GLfloat B);
	void SetTransparency(GLfloat Value);
	void SetCollisionArea(GLfloat X, GLfloat Y, GLfloat SizeX, GLfloat SizeY);
	bool CheckCursorCollision(GLfloat X, GLfloat Y);
	void InitTransform();
	void Render(GLfloat Width = 1.0, GLfloat Height = 1.0, ButtonFlip FlipOption=(ButtonFlip)(-1));

private:
	void ProcessTransform();
};


class Cursor {
private:
	unsigned int ModelLocation{}, TransparencyLocation{}, ObjectColorLocation{}, TextAlphaLocation{};
	unsigned int Image{};

	glm::mat4 TranslateMatrix{ 1.0f };
	glm::vec3 CursorColor{ 1.0f, 1.0f, 1.0f };
	GLfloat Transparency{ 1.0f };

public:
	void Init(std::string ImageName);
	void SetColor(GLfloat R, GLfloat G, GLfloat B);
	void SetTransparency(GLfloat Value);
	void Move(GLfloat X, GLfloat Y);
	void Rotate(GLfloat RotationValue);
	void Scale(GLfloat SizeH, GLfloat SizeV);
	void InitTransform();
	void Render(GLfloat Width=1.0f, GLfloat Height=1.0f, CursorFlip FlipOption=(CursorFlip(-1)));

private:
	void ProcessTransform();
};