#pragma once
#include "EngineHeader.h"

enum class CollisionEdge
{Left, Right, Top, Bottom};

class AABB {
private:
	unsigned int ModelLocation{}, TransparencyLocation{}, ObjectColorLocation{};
	unsigned int Box{};
	unsigned int BoxInside{};

	GLfloat LeftX{}, LeftY{};
	GLfloat RightX{}, RightY{};
	GLfloat OffsetX{}, OffsetY{};

	bool Collide{};

public:
	glm::mat4 TranslateMatrix{ 1.0f }, ScaleMatrix{ 1.0f }, RotateMatrix{ 1.0f };
	GLfloat Width{}, Height{};
	glm::vec2 Center{};

	void Update(GLfloat X, GLfloat Y, GLfloat xScale, GLfloat yScale);
	void InitMatrix();
	void Move(glm::mat4& Matrix, GLfloat X, GLfloat Y);
	void Rotate(glm::mat4& Matrix, GLfloat Rotation);
	void Scale(glm::mat4& Matrix, GLfloat X, GLfloat Y);
	void Render();
	bool CheckCollision(const AABB& Other);
	bool CheckCollisionEdge(GLfloat X, CollisionEdge Edge);
	void InterpolateX(GLfloat X);
	void InterpolateY(GLfloat Y);
	bool CheckCollisionPoint(GLfloat X, GLfloat Y);

private:
	void ProcessTransform();
};

class OBB {
private:
	unsigned int ModelLocation{}, TransparencyLocation{}, ObjectColorLocation{};
	unsigned int Box{};
	unsigned int BoxInside{};

	glm::vec2 Offset{};
	glm::vec2 Axis[2]{};

	bool Collide{};

public:
	glm::mat4 TranslateMatrix{ 1.0f }, RotateMatrix{ 1.0f }, ScaleMatrix{ 1.0f };
	GLfloat Width{}, Height{};
	GLfloat Rotation{};
	glm::vec2 Center{};

	void Update(GLfloat X, GLfloat Y, GLfloat BoxWidth, GLfloat BoxHeight, GLfloat RotationValue);
	void InitMatrix();
	void Move(glm::mat4& Matrix, GLfloat X, GLfloat Y);
	void Rotate(glm::mat4& Matrix, GLfloat Rotation);
	void Scale(glm::mat4& Matrix, GLfloat X, GLfloat Y);
	void Render();
	bool CheckCollision(const OBB& Other);
	bool CheckCollisionPoint(GLfloat X, GLfloat Y);

private:
	bool OverlapOnAxis(const OBB& OBB1, const OBB& OBB2, const glm::vec2& Axis);
	void ProcessTransform();
};

class Range {
private:
	unsigned int ModelLocation{}, TransparencyLocation{}, ObjectColorLocation{};
	unsigned int Circle{};
	unsigned int CircleInside{};

	bool Collide{};

public:
	glm::mat4 TranslateMatrix{ 1.0f }, ScaleMatrix{ 1.0f }, RotateMatrix{ 1.0f };
	glm::vec2 Center{};
	GLfloat Radius{};

	void Update(GLfloat X, GLfloat Y, GLfloat Size);
	void InitMatrix();
	void Move(glm::mat4& Matrix, GLfloat X, GLfloat Y);
	void Rotate(glm::mat4& Matrix, GLfloat Rotation);
	void Scale(glm::mat4& Matrix, GLfloat X, GLfloat Y);
	void Render();
	bool CheckCollision(const Range& Other);
	bool CheckCollisionPoint(GLfloat X, GLfloat Y);

private:
	GLfloat CalculateDistance(GLfloat x2, GLfloat y2);
	void ProcessTransform();
};