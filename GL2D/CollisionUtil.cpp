#include "CollisionUtil.h"
#include "RenderModeUtil.h"
#include "ImageUtil.h"
#include "CameraUtil.h"


void AABB::Init() {
	if (ShowBoundBox)
		imageUtil.SetImage(Box, "BOUNDBOX");
}

void AABB::Update(GLfloat X, GLfloat Y, GLfloat xScale, GLfloat yScale) {
	OffsetX = xScale / 2;
	OffsetY = yScale / 2;

	LeftX = X - OffsetX;
	RightX = X + OffsetX;
	LeftY = Y - OffsetY;
	RightY = Y + OffsetY;

	CenterX = X;
	CenterY = Y;

	if (ShowBoundBox) {
		InitTransform();

		TranslateMatrix = translate(TranslateMatrix, glm::vec3(X, Y, 0.0));
		TranslateMatrix = scale(TranslateMatrix, glm::vec3(xScale, yScale, 0.0));

		ProcessTransform();
		imageUtil.Render(Box);
	}
}

bool AABB::CheckCollisionAABB(const AABB& Other) {
	if (RightX < Other.LeftX || LeftX > Other.RightX)
		return false;

	if (RightY < Other.LeftY || LeftY > Other.RightY)
		return false;

	return true;
}

bool AABB::CheckCollisionEdge(GLfloat Value, Edge Edge) {
	switch (Edge) {
	case Edge::Right:
		if (RightX > Value) {
			CenterX = RightX - (RightX - Value) - OffsetX;
			return true;
		}
		break;

	case Edge::Left:
		if (LeftX < Value) {
			CenterX = LeftX + (Value - LeftX) + OffsetX;
			return true;
		}
		break;

	case Edge::Top:
		if (RightY > Value) {
			CenterY = RightY - (RightY - Value) - OffsetY;
			return true;
		}
		break;

	case Edge::Bottom:
		if (LeftY < Value) {
			CenterY = LeftY + (Value - LeftY) + OffsetY;
			return true;
		}
	}

	return false;
}

bool AABB::CheckCollisionPoint(GLfloat X, GLfloat Y) {
	if ((LeftX <= X && X <= RightX) && (LeftY <= Y && Y <= RightY))
		return true;

	return false;
}

void AABB::InterpolateX(GLfloat& X) {
	X = CenterX;
}

void AABB::InterpolateY(GLfloat& Y) {
	Y = CenterY;
}

void AABB::InitTransform() {
	TranslateMatrix = glm::mat4(1.0f);
}

void AABB::ProcessTransform() {
	renderMode.SetImageMode();

	TransparencyLocation = glGetUniformLocation(ImageShader, "transparency");
	glUniform1f(TransparencyLocation, 1.0);

	ObjectColorLocation = glGetUniformLocation(TextShader, "objectColor");
	glUniform3f(ObjectColorLocation, 1.0, 0.0, 0.0);

	ModelLocation = glGetUniformLocation(ImageShader, "model");
	glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, value_ptr(TranslateMatrix));
}



void OBB::Init() {
	if (ShowBoundBox)
		imageUtil.SetImage(Box, "BOUNDBOX");
}

void OBB::Update(GLfloat X, GLfloat Y, GLfloat xScale, GLfloat yScale, float Degree) {
	Center = glm::vec2(X, Y);
	Offset = glm::vec2(xScale / 2, yScale / 2);
	Rotation = Degree;

	float Rad = glm::radians(Degree);
	Axis[0] = glm::vec2(std::cos(Rad), std::sin(Rad));
	Axis[1] = glm::vec2(-std::sin(Rad), std::cos(Rad));

	if (ShowBoundBox) {
		InitTransform();

		TranslateMatrix = translate(TranslateMatrix, glm::vec3(X, Y, 0.0f));
		TranslateMatrix = rotate(TranslateMatrix, Rad, glm::vec3(0.0f, 0.0f, 1.0f));
		TranslateMatrix = scale(TranslateMatrix, glm::vec3(xScale, yScale, 1.0f));

		ProcessTransform();
		imageUtil.Render(Box);
	}
}

bool OBB::OverlapOnAxis(const OBB& OBB1, const OBB& OBB2, const glm::vec2& Axis) {
	auto Project = [](const OBB& OBB, const glm::vec2& Axis) {
		glm::vec2 Corners[4] = {
			OBB.Center + OBB.Axis[0] * OBB.Offset.x + OBB.Axis[1] * OBB.Offset.y,
			OBB.Center + OBB.Axis[0] * OBB.Offset.x - OBB.Axis[1] * OBB.Offset.y,
			OBB.Center - OBB.Axis[0] * OBB.Offset.x + OBB.Axis[1] * OBB.Offset.y,
			OBB.Center - OBB.Axis[0] * OBB.Offset.x - OBB.Axis[1] * OBB.Offset.y
		};

		float Min = glm::dot(Corners[0], Axis);
		float Max = Min;

		for (int i = 1; i < 4; i++) {
			float Projection = glm::dot(Corners[i], Axis);

			if (Projection < Min) 
				Min = Projection;

			if (Projection > Max) 
				Max = Projection;
		}

		return std::make_pair(Min, Max);
	};

	auto [Min1, Max1] = Project(OBB1, Axis);
	auto [Min2, Max2] = Project(OBB2, Axis);

	return !(Max1 < Min2 || Max2 < Min1);
}

bool OBB::CheckCollisionOBB(const OBB& Other) {
	glm::vec2 Axises[] = { Axis[0], Axis[1], Other.Axis[0], Other.Axis[1] };

	for (const auto& Axis : Axises) {
		if (!OverlapOnAxis(*this, Other, Axis))
			return false;
	}

	return true;
}

bool OBB::CheckCollisionPoint(const glm::vec2& Point) {
	glm::vec2 D = Point - Center;

	for (int i = 0; i < 2; ++i) {
		float Dist = glm::dot(D, Axis[i]);
		if (std::abs(Dist) > Offset[i])
			return false;
	}
	return true;
}

void OBB::InitTransform() {
	TranslateMatrix = glm::mat4(1.0f);
}

void OBB::ProcessTransform() {
	renderMode.SetImageMode();

	TransparencyLocation = glGetUniformLocation(ImageShader, "transparency");
	glUniform1f(TransparencyLocation, 1.0);

	ObjectColorLocation = glGetUniformLocation(TextShader, "objectColor");
	glUniform3f(ObjectColorLocation, 1.0, 0.0, 0.0);

	ModelLocation = glGetUniformLocation(ImageShader, "model");
	glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, value_ptr(TranslateMatrix));
}



void Range::Init() {
	if (ShowBoundBox)
		imageUtil.SetImage(Circle, "CIRCLE");
}

void Range::InitTransform() {
	TranslateMatrix = glm::mat4(1.0f);
}

void Range::Update(GLfloat X, GLfloat Y, GLfloat Size) {
	CenterX = X;
	CenterY = Y;
	Radius = Size / 2.0;

	if (ShowBoundBox) {
		InitTransform();
		TranslateMatrix = translate(TranslateMatrix, glm::vec3(X, Y, 0.0));
		TranslateMatrix = scale(TranslateMatrix, glm::vec3(Size, Size, 1.0));

		ProcessTransform();
		imageUtil.Render(Circle);
	}
}

GLfloat Range::CalculateDistance( GLfloat X, GLfloat Y) {
	GLfloat DX = X - CenterX;
	GLfloat DY = Y - CenterY;
	return std::sqrt(DX * DX + DY * DY);
}

bool Range::CheckCollisionRange(const Range& Other) {
	if (CalculateDistance(Other.CenterX, Other.CenterY) < Radius + Other.Radius)
		return true;

	return false;
}

bool Range::CheckCollisionPoint(GLfloat X, GLfloat Y) {
	if (CalculateDistance(X, Y) < Radius)
		return true;

	return false;
}

void Range::ProcessTransform() {
	renderMode.SetImageMode();

	TransparencyLocation = glGetUniformLocation(ImageShader, "transparency");
	glUniform1f(TransparencyLocation, 1.0);

	ObjectColorLocation = glGetUniformLocation(TextShader, "objectColor");
	glUniform3f(ObjectColorLocation, 1.0, 0.0, 0.0);

	ModelLocation = glGetUniformLocation(ImageShader, "model");
	glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, value_ptr(TranslateMatrix));
}