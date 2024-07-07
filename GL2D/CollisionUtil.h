#pragma once
#include "MATA_HEADER.h"

enum class Edge
{Left, Right, Top, Bottom};

class AABB {
private:
	glm::mat4 TranslateMatrix{ 1.0f }, ScaleMatrix{ 1.0f };
	unsigned int ModelLocation{}, TransparencyLocation{}, ObjectColorLocation{};

	GLfloat LeftX{}, LeftY{};
	GLfloat RightX{}, RightY{};
	GLfloat CenterX{}, CenterY{};
	GLfloat OffsetX{}, OffsetY{};

	unsigned int Box{};

public:
	void Init();
	void Update(GLfloat X, GLfloat Y, GLfloat xScale, GLfloat yScale);
	bool CheckCollisionAABB(const AABB& AABB);
	bool CheckCollisionEdge(GLfloat X, Edge Edge);
	void InterpolateX(GLfloat& X);
	void InterpolateY(GLfloat& Y);
	bool CheckCollisionPoint(GLfloat X, GLfloat Y);
	void InitTransform();
	void ProcessTransform();
};

class OBB {
private:
	glm::mat4 TranslateMatrix{ 1.0f }, ScaleMatrix{ 1.0f }, RotateMatrix{ 1.0f };
	unsigned int ModelLocation{}, TransparencyLocation{}, ObjectColorLocation{};

	glm::vec2 Center;
	glm::vec2 Offset;
	glm::vec2 Axis[2];
	GLfloat Rotation;

	unsigned int Box{};

public:
	void Init();
	void Update(GLfloat X, GLfloat Y, GLfloat xScale, GLfloat yScale, float rotation);
	void InitTransform();
	bool CheckCollisionOBB(const OBB& OBB);
	bool CheckCollisionPoint(const glm::vec2& Point);
	void ProcessTransform();

private:
	bool OverlapOnAxis(const OBB& OBB1, const OBB& OBB2, const glm::vec2& Axis);
};