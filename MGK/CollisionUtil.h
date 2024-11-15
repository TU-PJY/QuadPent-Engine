#pragma once
#include "EngineHeader.h"
#include "RectBrush.h"
#include "CircleBrush.h"

class AABB {
private:
	GLfloat LeftX{}, LeftY{};
	GLfloat RightX{}, RightY{};
	GLfloat OffsetX{}, OffsetY{};

	glm::vec2 Center{};
	GLfloat Width{}, Height{};

	bool Collide{};

#ifdef SHOW_BOUND_BOX
	LineRectBrush LineRect{true};
	RectBrush Rect{true};
#endif

public:
	void Update(GLfloat X, GLfloat Y, GLfloat xScale, GLfloat yScale);
	void Update(glm::vec2 Position, GLfloat xScale, GLfloat yScale);
	void Render();
	bool CheckCollision(const AABB& Other);
	bool CheckCollisionPoint(GLfloat X, GLfloat Y);
};

class OOBB {
private:
	glm::vec2 Offset{};
	glm::vec2 Axis[2]{};
	glm::vec2 Corners[4]{};

	glm::vec2 Center{};
	GLfloat Width{}, Height{};
	GLfloat Rotation{};

	bool Collide{};

#ifdef SHOW_BOUND_BOX
	LineRectBrush LineRect{ true };
	RectBrush Rect{ true };
#endif

public:
	void Update(GLfloat X, GLfloat Y, GLfloat BoxWidth, GLfloat BoxHeight, GLfloat RotationValue);
	void Update(glm::vec2 Position, GLfloat BoxWidth, GLfloat BoxHeight, GLfloat RotationValue);
	void Render();
	bool CheckCollision(const OOBB& Other);
	bool CheckCollisionPoint(GLfloat X, GLfloat Y);

private:
	std::pair<float, float> Project(const OOBB& OBB, const glm::vec2& Axis);
	bool OverlapOnAxis(const OOBB& OBB1, const OOBB& OBB2, const glm::vec2& Axis);
};

class BoundingCircle {
private:
	glm::vec2 Center{};
	GLfloat Radius{};
	GLfloat Size{};

	bool Collide{};

#ifdef SHOW_BOUND_BOX
	LineCircleBrush LineCircle { true };
	CircleBrush Circle{ true };
#endif


public:
	void Update(GLfloat X, GLfloat Y, GLfloat Diameter);
	void Update(glm::vec2 Position, GLfloat SizeValue);
	void Render();
	bool CheckCollision(const BoundingCircle& Other);
	bool CheckCollisionPoint(GLfloat X, GLfloat Y);
};