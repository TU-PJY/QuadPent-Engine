#include "CollisionUtil.h"
#include "TransformUtil.h"
#include "ImageUtil.h"
#include "CameraUtil.h"
#include "SystemResource.h"
#include "MathUtil.h"


void SDK::AABB::Update(GLfloat X, GLfloat Y, GLfloat xScale, GLfloat yScale) {
	aabb.Center = XMFLOAT3(X, Y, 0.0);
	aabb.Extents = XMFLOAT3(xScale / 2.0, yScale / 2.0, 0.0);
}

void SDK::AABB::Update(glm::vec2& Position, GLfloat xScale, GLfloat yScale) {
	aabb.Center = XMFLOAT3(Position.x, Position.y, 0.0);
	aabb.Extents = XMFLOAT3(xScale / 2.0, yScale / 2.0, 0.0);
}

void SDK::AABB::Render() {
	if (SHOW_BOUND_BOX) {
		LineRect.SetColor(1.0, 0.0, 0.0);
		RECT.SetColor(1.0, 0.0, 0.0);

		LineRect.Draw(aabb.Center.x, aabb.Center.y, aabb.Extents.x * 2.0, aabb.Extents.y * 2.0, BOUND_BOX_THICKNESS, 0.0);
		if (Collide)
			RECT.Draw(aabb.Center.x, aabb.Center.y, aabb.Extents.x * 2.0, aabb.Extents.y * 2.0, 0.0, 0.3);
	}
}

bool SDK::AABB::CheckCollision(const AABB& Other) {
	if (aabb.Intersects(Other.aabb)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool SDK::AABB::CheckCollision(const OOBB& Other) {
	if (aabb.Intersects(Other.Get())) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool SDK::AABB::CheckCollision(const BoundingCircle& Other) {
	if (aabb.Intersects(Other.Get())) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool SDK::AABB::CheckCollisionPoint(GLfloat X, GLfloat Y) {
	XMFLOAT3 Point(X, Y, 0.0);
	if (aabb.Contains(XMLoadFloat3(&Point)) == ContainmentType::CONTAINS) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool SDK::AABB::CheckCollisionPoint(glm::vec2& Position) {
	XMFLOAT3 Point(Position.x, Position.y, 0.0);
	if (aabb.Contains(XMLoadFloat3(&Point)) == ContainmentType::CONTAINS) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool SDK::AABB::CheckCollisionRay(RayVector& Ray) {
	if (aabb.Intersects(Ray.Origin, Ray.Direction, Ray.Distance)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool SDK::AABB::CheckCollisionLine(RayVector& Ray) {
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

BoundingBox SDK::AABB::Get() const{
	return aabb;
}



void SDK::OOBB::Update(GLfloat X, GLfloat Y, GLfloat BoxWidth, GLfloat BoxHeight, GLfloat RotationValue) {
	oobb.Center = XMFLOAT3(X, Y, 0.0);
	oobb.Extents = XMFLOAT3(BoxWidth * 0.5, BoxHeight * 0.5, 0.0);
	XMVECTOR Quaternion = XMQuaternionRotationRollPitchYaw(0.0, 0.0, XMConvertToRadians(-RotationValue));
	XMStoreFloat4(&oobb.Orientation, Quaternion);
	Rotation = RotationValue;
}

void SDK::OOBB::Update(glm::vec2& Position, GLfloat BoxWidth, GLfloat BoxHeight, GLfloat RotationValue) {
	oobb.Center = XMFLOAT3(Position.x, Position.y, 0.0);
	oobb.Extents = XMFLOAT3(BoxWidth * 0.5, BoxHeight * 0.5, 0.0);
	XMVECTOR Quaternion = XMQuaternionRotationRollPitchYaw(0.0, 0.0, XMConvertToRadians(-RotationValue));
	XMStoreFloat4(&oobb.Orientation, Quaternion);
	Rotation = RotationValue;
}

void SDK::OOBB::Render() {
	if (SHOW_BOUND_BOX) {
		LineRect.SetColor(1.0, 0.0, 0.0);
		RECT.SetColor(1.0, 0.0, 0.0);

		LineRect.Draw(oobb.Center.x, oobb.Center.y, oobb.Extents.x * 2.0, oobb.Extents.y * 2.0, BOUND_BOX_THICKNESS, Rotation);
		if (Collide)
			RECT.Draw(oobb.Center.x, oobb.Center.y, oobb.Extents.x * 2.0, oobb.Extents.y * 2.0, Rotation, 0.3);
	}
}

bool SDK::OOBB::CheckCollision(const OOBB& Other) {
	if (oobb.Intersects(Other.oobb)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool SDK::OOBB::CheckCollision(const AABB& Other) {
	if (oobb.Intersects(Other.Get())) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool SDK::OOBB::CheckCollision(const BoundingCircle& Other) {
	if (oobb.Intersects(Other.Get())) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool SDK::OOBB::CheckCollisionPoint(GLfloat X, GLfloat Y) {
	XMFLOAT3 Point(X, Y, 0.0);
	if (oobb.Contains(XMLoadFloat3(&Point)) == ContainmentType::CONTAINS) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool SDK::OOBB::CheckCollisionPoint(glm::vec2& Position) {
	XMFLOAT3 Point(Position.x, Position.y, 0.0);
	if (oobb.Contains(XMLoadFloat3(&Point)) == ContainmentType::CONTAINS) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool SDK::OOBB::CheckCollisionRay(RayVector& Ray) {
	if (oobb.Intersects(Ray.Origin, Ray.Direction, Ray.Distance)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}


bool SDK::OOBB::CheckCollisionLine(RayVector& Ray) {
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

BoundingOrientedBox SDK::OOBB::Get() const {
	return oobb;
}



void SDK::BoundingCircle::Update(GLfloat X, GLfloat Y, GLfloat SizeValue) {
	sphere.Center = XMFLOAT3(X, Y, 0.0);
	sphere.Radius = SizeValue * 0.5;
}

void SDK::BoundingCircle::Update(glm::vec2& Position, GLfloat SizeValue) {
	sphere.Center = XMFLOAT3(Position.x, Position.y, 0.0);
	sphere.Radius = SizeValue * 0.5;
}

void SDK::BoundingCircle::Render() {
	if (SHOW_BOUND_BOX) {
		Circle.SetColor(1.0, 0.0, 0.0);
		LineCircle.SetColor(1.0, 0.0, 0.0);

		LineCircle.Draw(sphere.Center.x, sphere.Center.y, sphere.Radius * 2.0 - BOUND_BOX_THICKNESS, 0.01);
		if (Collide)
			Circle.Draw(sphere.Center.x, sphere.Center.y, sphere.Radius * 2.0, 0.3);
	}
}

bool SDK::BoundingCircle::CheckCollision(const BoundingCircle& Other) {
	if (sphere.Intersects(Other.sphere)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool SDK::BoundingCircle::CheckCollision(const AABB& Other) {
	if (sphere.Intersects(Other.Get())) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool SDK::BoundingCircle::CheckCollision(const OOBB& Other) {
	if (sphere.Intersects(Other.Get())) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool SDK::BoundingCircle::CheckCollisionPoint(GLfloat X, GLfloat Y) {
	XMFLOAT3 Point(X, Y, 0.0);
	if (sphere.Contains(XMLoadFloat3(&Point)) == ContainmentType::CONTAINS) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool SDK::BoundingCircle::CheckCollisionPoint(glm::vec2& Position) {
	XMFLOAT3 Point(Position.x, Position.y, 0.0);
	if (sphere.Contains(XMLoadFloat3(&Point)) == ContainmentType::CONTAINS) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool SDK::BoundingCircle::CheckCollisionRay(RayVector& Ray) {
	if (sphere.Intersects(Ray.Origin, Ray.Direction, Ray.Distance)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool SDK::BoundingCircle::CheckCollisionLine(RayVector& Ray) {
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


BoundingSphere SDK::BoundingCircle::Get() const {
	return sphere;
}