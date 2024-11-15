#include "CollisionUtil.h"
#include "TransformUtil.h"
#include "ImageUtil.h"
#include "CameraUtil.h"
#include "SystemResource.h"
#include "MathUtil.h"


void AABB::Update(GLfloat X, GLfloat Y, GLfloat xScale, GLfloat yScale) {
	OffsetX = xScale / 2;
	OffsetY = yScale / 2;

	LeftX = X - OffsetX;
	RightX = X + OffsetX;
	LeftY = Y - OffsetY;
	RightY = Y + OffsetY;

	Center.x = X;
	Center.y = Y;

	Width = xScale;
	Height = yScale;
}

void AABB::Update(glm::vec2 Position, GLfloat xScale, GLfloat yScale) {
	OffsetX = xScale / 2;
	OffsetY = yScale / 2;

	LeftX = Position.x - OffsetX;
	RightX = Position.x + OffsetX;
	LeftY = Position.y - OffsetY;
	RightY = Position.y + OffsetY;

	Center.x = Position.x;
	Center.y = Position.y;

	Width = xScale;
	Height = yScale;
}

void AABB::Render() {
#ifdef SHOW_BOUND_BOX
	LineRect.SetColor(1.0, 0.0, 0.0);
	Rect.SetColor(1.0, 0.0, 0.0);

	LineRect.Draw(Center.x, Center.y, Width, Height, 0.01, 0.0);

	if (Collide)
		Rect.Draw(Center.x, Center.y, Width, Height, 0.0, 0.3);
#endif
}

bool AABB::CheckCollision(const AABB& Other) {
	if (RightX < Other.LeftX || LeftX > Other.RightX) {
		Collide = false;
		return false;
	}

	if (RightY < Other.LeftY || LeftY > Other.RightY) {
		Collide = false;
		return false;
	}
	
	Collide = true;
	return true;
}

bool AABB::CheckCollisionPoint(GLfloat X, GLfloat Y) {
	if ((LeftX <= X && X <= RightX) && (LeftY <= Y && Y <= RightY)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}



void OOBB::Update(GLfloat X, GLfloat Y, GLfloat BoxWidth, GLfloat BoxHeight, GLfloat RotationValue) {
	Center = glm::vec2(X, Y);
	Offset = glm::vec2(BoxWidth / 2, BoxHeight / 2);

	GLfloat Rad = glm::radians(RotationValue);
	Axis[0] = glm::vec2(std::cos(Rad), std::sin(Rad));
	Axis[1] = glm::vec2(-std::sin(Rad), std::cos(Rad));

	Width = BoxWidth;
	Height = BoxHeight;
	Rotation = RotationValue;
}

void OOBB::Update(glm::vec2 Position, GLfloat BoxWidth, GLfloat BoxHeight, GLfloat RotationValue) {
	Center = Position;
	Offset = glm::vec2(BoxWidth / 2, BoxHeight / 2);

	GLfloat Rad = glm::radians(RotationValue);
	Axis[0] = glm::vec2(std::cos(Rad), std::sin(Rad));
	Axis[1] = glm::vec2(-std::sin(Rad), std::cos(Rad));

	Width = BoxWidth;
	Height = BoxHeight;
	Rotation = RotationValue;
}

void OOBB::Render() {
#ifdef SHOW_BOUND_BOX
	LineRect.SetColor(1.0, 0.0, 0.0);
	Rect.SetColor(1.0, 0.0, 0.0);

	LineRect.Draw(Center.x, Center.y, Width, Height, 0.01, Rotation);

	if (Collide)
		Rect.Draw(Center.x, Center.y, Width, Height, Rotation, 0.3);
#endif
}

std::pair<float, float> OOBB::Project(const OOBB& oobb, const glm::vec2& Axis) {
	Corners[0] = oobb.Center + oobb.Axis[0] * oobb.Offset.x + oobb.Axis[1] * oobb.Offset.y;
	Corners[1] = oobb.Center + oobb.Axis[0] * oobb.Offset.x - oobb.Axis[1] * oobb.Offset.y;
	Corners[2] = oobb.Center - oobb.Axis[0] * oobb.Offset.x + oobb.Axis[1] * oobb.Offset.y;
	Corners[3] = oobb.Center - oobb.Axis[0] * oobb.Offset.x - oobb.Axis[1] * oobb.Offset.y;

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
}

bool OOBB::OverlapOnAxis(const OOBB& oobb1, const OOBB& oobb2, const glm::vec2& Axis) {
	auto [Min1, Max1] = Project(oobb1, Axis);
	auto [Min2, Max2] = Project(oobb2, Axis);

	return !(Max1 < Min2 || Max2 < Min1);
}

bool OOBB::CheckCollision(const OOBB& Other) {
	glm::vec2 Axises[] = { Axis[0], Axis[1], Other.Axis[0], Other.Axis[1] };

	for (const auto& Axis : Axises) {
		if (!OverlapOnAxis(*this, Other, Axis)) {
			Collide = false;
			return false;
		}
	}

	Collide = true;
	return true;
}

bool OOBB::CheckCollisionPoint(GLfloat X, GLfloat Y) {
	glm::vec2 D = glm::vec2(X, Y) - Center;

	for (int i = 0; i < 2; ++i) {
		GLfloat Dist = glm::dot(D, Axis[i]);
		if (std::abs(Dist) > Offset[i]) {
			Collide = false;
			return false;
		}
	}

	Collide = true;
	return true;
}



void BoundingCircle::Update(GLfloat X, GLfloat Y, GLfloat SizeValue) {
	Center.x = X;
	Center.y = Y;
	Radius = SizeValue * 0.5;
	Size = SizeValue;
}

void BoundingCircle::Update(glm::vec2 Position, GLfloat SizeValue) {
	Center = Position;
	Radius = SizeValue * 0.5;
	Size = SizeValue;
}

void BoundingCircle::Render() {
#ifdef SHOW_BOUND_BOX
	Circle.SetColor(1.0, 0.0, 0.0);
	LineCircle.SetColor(1.0, 0.0, 0.0);

	LineCircle.Draw(Center.x, Center.y, Size, 0.01);

	if (Collide)
		Circle.Draw(Center.x, Center.y, Size, 0.3);
#endif
}

bool BoundingCircle::CheckCollision(const BoundingCircle& Other) {
	if (Math::CalcDistance(Center.x, Center.y, Other.Center.x, Other.Center.y) < Radius + Other.Radius) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool BoundingCircle::CheckCollisionPoint(GLfloat X, GLfloat Y) {
	if (Math::CalcDistance(Center.x, Center.y, X, Y) < Radius) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}