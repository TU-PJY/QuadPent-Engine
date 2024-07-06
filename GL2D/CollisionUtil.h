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
	void Update(GLfloat X, GLfloat Y, GLfloat xScale, GLfloat yScale, bool UseViewportPosition = false);
	bool CheckCollisionAABB(AABB aabb);
	bool CheckCollisionEdge(GLfloat X, Edge Edge);
	void InterpolateX(GLfloat& X);
	void InterpolateY(GLfloat& Y);
	bool CheckCollisionDot(GLfloat X, GLfloat Y);
	void InitTransform();
	void ProcessTransform();
};