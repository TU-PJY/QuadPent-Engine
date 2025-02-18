#include "SDK_Collision.h"

void MSDK::AABB::Update(float X, float Y, float xScale, float yScale) {
	aabb.Center = XMFLOAT3(X, Y, 0.0);
	aabb.Extents = XMFLOAT3(xScale / 2.0, yScale / 2.0, 0.0);
}

void MSDK::AABB::Update(glm::vec2& Position, float xScale, float yScale) {
	aabb.Center = XMFLOAT3(Position.x, Position.y, 0.0);
	aabb.Extents = XMFLOAT3(xScale / 2.0, yScale / 2.0, 0.0);
}

void MSDK::AABB::Render() {
	if (SHOW_BOUND_BOX) {
		LineRect.SetColor(1.0, 0.0, 0.0);
		RECT.SetColor(1.0, 0.0, 0.0);

		LineRect.Draw(aabb.Center.x, aabb.Center.y, aabb.Extents.x * 2.0, aabb.Extents.y * 2.0, BOUND_BOX_THICKNESS, 0.0);
		if (Collide)
			RECT.Draw(aabb.Center.x, aabb.Center.y, aabb.Extents.x * 2.0, aabb.Extents.y * 2.0, 0.0, 0.3);
	}
}

bool MSDK::AABB::CheckCollision(const AABB& Other) {
	if (aabb.Intersects(Other.aabb)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool MSDK::AABB::CheckCollision(const OOBB& Other) {
	if (aabb.Intersects(Other.Get())) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool MSDK::AABB::CheckCollision(const BoundingCircle& Other) {
	if (aabb.Intersects(Other.Get())) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool MSDK::AABB::CheckCollisionPoint(float X, float Y) {
	XMFLOAT3 Point(X, Y, 0.0);
	if (aabb.Contains(XMLoadFloat3(&Point)) == ContainmentType::CONTAINS) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool MSDK::AABB::CheckCollisionPoint(glm::vec2& Position) {
	XMFLOAT3 Point(Position.x, Position.y, 0.0);
	if (aabb.Contains(XMLoadFloat3(&Point)) == ContainmentType::CONTAINS) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool MSDK::AABB::CheckCollisionRay(MSDK::RayVector& Ray) {
	if (aabb.Intersects(Ray.Origin, Ray.Direction, Ray.Distance)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool MSDK::AABB::CheckCollisionLine(MSDK::RayVector& Ray) {
	if (aabb.Intersects(Ray.Origin, Ray.Direction, Ray.Distance)) {
		if (Ray.Distance <= Ray.Length) {
			Collide = true;
			return true;
		}
		else {
			Collide = false;
			return false;
		}
	}

	Collide = false;
	return false;
}

BoundingBox MSDK::AABB::Get() const{
	return aabb;
}



void MSDK::OOBB::Update(float X, float Y, float BoxWidth, float BoxHeight, float RotationValue) {
	oobb.Center = XMFLOAT3(X, Y, 0.0);
	oobb.Extents = XMFLOAT3(BoxWidth * 0.5, BoxHeight * 0.5, 0.0);
	XMVECTOR Quaternion = XMQuaternionRotationRollPitchYaw(0.0, 0.0, XMConvertToRadians(-RotationValue));
	XMStoreFloat4(&oobb.Orientation, Quaternion);
	Rotation = RotationValue;
}

void MSDK::OOBB::Update(glm::vec2& Position, float BoxWidth, float BoxHeight, float RotationValue) {
	oobb.Center = XMFLOAT3(Position.x, Position.y, 0.0);
	oobb.Extents = XMFLOAT3(BoxWidth * 0.5, BoxHeight * 0.5, 0.0);
	XMVECTOR Quaternion = XMQuaternionRotationRollPitchYaw(0.0, 0.0, XMConvertToRadians(-RotationValue));
	XMStoreFloat4(&oobb.Orientation, Quaternion);
	Rotation = RotationValue;
}

void MSDK::OOBB::Render() {
	if (SHOW_BOUND_BOX) {
		LineRect.SetColor(1.0, 0.0, 0.0);
		RECT.SetColor(1.0, 0.0, 0.0);

		LineRect.Draw(oobb.Center.x, oobb.Center.y, oobb.Extents.x * 2.0, oobb.Extents.y * 2.0, BOUND_BOX_THICKNESS, Rotation);
		if (Collide)
			RECT.Draw(oobb.Center.x, oobb.Center.y, oobb.Extents.x * 2.0, oobb.Extents.y * 2.0, Rotation, 0.3);
	}
}

bool MSDK::OOBB::CheckCollision(const OOBB& Other) {
	if (oobb.Intersects(Other.oobb)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool MSDK::OOBB::CheckCollision(const AABB& Other) {
	if (oobb.Intersects(Other.Get())) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool MSDK::OOBB::CheckCollision(const BoundingCircle& Other) {
	if (oobb.Intersects(Other.Get())) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool MSDK::OOBB::CheckCollisionPoint(float X, float Y) {
	XMFLOAT3 Point(X, Y, 0.0);
	if (oobb.Contains(XMLoadFloat3(&Point)) == ContainmentType::CONTAINS) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool MSDK::OOBB::CheckCollisionPoint(glm::vec2& Position) {
	XMFLOAT3 Point(Position.x, Position.y, 0.0);
	if (oobb.Contains(XMLoadFloat3(&Point)) == ContainmentType::CONTAINS) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool MSDK::OOBB::CheckCollisionRay(MSDK::RayVector& Ray) {
	if (oobb.Intersects(Ray.Origin, Ray.Direction, Ray.Distance)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}


bool MSDK::OOBB::CheckCollisionLine(MSDK::RayVector& Ray) {
	if (oobb.Intersects(Ray.Origin, Ray.Direction, Ray.Distance)) {
		if (Ray.Distance <= Ray.Length) {
			Collide = true;
			return true;
		}
		else {
			Collide = false;
			return false;
		}
	}

	Collide = false;
	return false;
}

BoundingOrientedBox MSDK::OOBB::Get() const {
	return oobb;
}



void MSDK::BoundingCircle::Update(float X, float Y, float SizeValue) {
	sphere.Center = XMFLOAT3(X, Y, 0.0);
	sphere.Radius = SizeValue * 0.5;
}

void MSDK::BoundingCircle::Update(glm::vec2& Position, float SizeValue) {
	sphere.Center = XMFLOAT3(Position.x, Position.y, 0.0);
	sphere.Radius = SizeValue * 0.5;
}

void MSDK::BoundingCircle::Render() {
	if (SHOW_BOUND_BOX) {
		Circle.SetColor(1.0, 0.0, 0.0);
		LineCircle.SetColor(1.0, 0.0, 0.0);

		LineCircle.Draw(sphere.Center.x, sphere.Center.y, sphere.Radius * 2.0 - BOUND_BOX_THICKNESS, 0.01);
		if (Collide)
			Circle.Draw(sphere.Center.x, sphere.Center.y, sphere.Radius * 2.0, 0.3);
	}
}

bool MSDK::BoundingCircle::CheckCollision(const BoundingCircle& Other) {
	if (sphere.Intersects(Other.sphere)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool MSDK::BoundingCircle::CheckCollision(const AABB& Other) {
	if (sphere.Intersects(Other.Get())) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool MSDK::BoundingCircle::CheckCollision(const OOBB& Other) {
	if (sphere.Intersects(Other.Get())) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool MSDK::BoundingCircle::CheckCollisionPoint(float X, float Y) {
	XMFLOAT3 Point(X, Y, 0.0);
	if (sphere.Contains(XMLoadFloat3(&Point)) == ContainmentType::CONTAINS) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool MSDK::BoundingCircle::CheckCollisionPoint(glm::vec2& Position) {
	XMFLOAT3 Point(Position.x, Position.y, 0.0);
	if (sphere.Contains(XMLoadFloat3(&Point)) == ContainmentType::CONTAINS) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool MSDK::BoundingCircle::CheckCollisionRay(MSDK::RayVector& Ray) {
	if (sphere.Intersects(Ray.Origin, Ray.Direction, Ray.Distance)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool MSDK::BoundingCircle::CheckCollisionLine(MSDK::RayVector& Ray) {
	if (sphere.Intersects(Ray.Origin, Ray.Direction, Ray.Distance)) {
		if (Ray.Distance <= Ray.Length) {
			Collide = true;
			return true;
		}
		else {
			Collide = false;
			return false;
		}
	}

	Collide = false;
	return false;
}


BoundingSphere MSDK::BoundingCircle::Get() const {
	return sphere;
}