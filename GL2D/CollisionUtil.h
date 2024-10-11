#pragma once
#include "EngineHeader.h"
#include "RectBrush.h"

class AABB {
private:
	unsigned int ModelLocation{}, TransparencyLocation{}, ObjectColorLocation{};
	glm::mat4 TranslateMatrix{ 1.0f }, ScaleMatrix{ 1.0f };

	GLfloat LeftX{}, LeftY{};
	GLfloat RightX{}, RightY{};
	GLfloat OffsetX{}, OffsetY{};

	glm::vec2 Center{};
	GLfloat Width{}, Height{};

	bool Collide{};

#ifdef SHOW_BOUND_BOX
	LineRectBrush LineRect;
	RectBrush Rect;
#endif

public:
	void Update(GLfloat X, GLfloat Y, GLfloat xScale, GLfloat yScale);
	void Render();
	bool CheckCollision(const AABB& Other);
	bool CheckCollisionPoint(GLfloat X, GLfloat Y);
};

class OBB {
private:
	unsigned int ModelLocation{}, TransparencyLocation{}, ObjectColorLocation{};
	glm::mat4 TranslateMatrix{ 1.0f }, RotateMatrix{ 1.0f }, ScaleMatrix{ 1.0f };

	glm::vec2 Offset{};
	glm::vec2 Axis[2]{};
	glm::vec2 Corners[4]{};

	glm::vec2 Center{};
	GLfloat Width{}, Height{};
	GLfloat Rotation{};

	bool Collide{};

#ifdef SHOW_BOUND_BOX
	LineRectBrush LineRect;
	RectBrush Rect;
#endif

public:
	void Update(GLfloat X, GLfloat Y, GLfloat BoxWidth, GLfloat BoxHeight, GLfloat RotationValue);
	void Render();
	bool CheckCollision(const OBB& Other);
	bool CheckCollisionPoint(GLfloat X, GLfloat Y);

private:
	std::pair<float, float> Project(const OBB& OBB, const glm::vec2& Axis);
	bool OverlapOnAxis(const OBB& OBB1, const OBB& OBB2, const glm::vec2& Axis);
};

class Range {
private:
	unsigned int ModelLocation{}, TransparencyLocation{}, ObjectColorLocation{};
	glm::mat4 TranslateMatrix{ 1.0f }, ScaleMatrix{ 1.0f };

	glm::vec2 Center{};
	GLfloat Radius{};
	GLfloat Size{};

	bool Collide{};

public:
	void Update(GLfloat X, GLfloat Y, GLfloat SizeValue);
	void Render();
	bool CheckCollision(const Range& Other);
	bool CheckCollisionPoint(GLfloat X, GLfloat Y);

private:
	GLfloat CalculateDistance(GLfloat x2, GLfloat y2);
	void ProcessTransform();
};