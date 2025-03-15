#include "QuadPent_Collision.h"

void QP::AABB::Update(float X, float Y, float xScale, float yScale) {
	aabb.Center = XMFLOAT3(X, Y, 0.0);
	aabb.Extents = XMFLOAT3(xScale / 2.0, yScale / 2.0, 0.0);
}

void QP::AABB::Update(QP::Vector2& Position, float xScale, float yScale) {
	aabb.Center = XMFLOAT3(Position.x, Position.y, 0.0);
	aabb.Extents = XMFLOAT3(xScale / 2.0, yScale / 2.0, 0.0);
}

void QP::AABB::Render() {
	if (ENABLE_DEV_MODE && ENABLE_RENDER_BOUND_BOX) {
		LineRect.SetColor(1.0, 0.0, 0.0);
		RECT.SetColor(1.0, 0.0, 0.0);

		LineRect.Render(aabb.Center.x, aabb.Center.y, aabb.Extents.x * 2.0, aabb.Extents.y * 2.0, Thickness, 0.0);
		if (Collide)
			RECT.Render(aabb.Center.x, aabb.Center.y, aabb.Extents.x * 2.0, aabb.Extents.y * 2.0, 0.0, 0.3);
	}
}

void QP::AABB::SetThickness(float Value) {
	if (Value <= 0.0)
		return;
	Thickness = Value;
}

bool QP::AABB::CheckCollision(const AABB& Other) {
	if (aabb.Intersects(Other.aabb)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool QP::AABB::CheckCollision(const OOBB& Other) {
	if (aabb.Intersects(Other.Get())) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool QP::AABB::CheckCollision(const BoundingCircle& Other) {
	if (aabb.Intersects(Other.Get())) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool QP::AABB::CheckCollisionPoint(float X, float Y) {
	XMFLOAT3 Point(X, Y, 0.0);
	if (aabb.Contains(XMLoadFloat3(&Point)) == ContainmentType::CONTAINS) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool QP::AABB::CheckCollisionPoint(QP::Vector2& Position) {
	XMFLOAT3 Point(Position.x, Position.y, 0.0);
	if (aabb.Contains(XMLoadFloat3(&Point)) == ContainmentType::CONTAINS) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool QP::AABB::CheckCollisionRay(QP::RayVector& Ray) {
	if (aabb.Intersects(Ray.Origin, Ray.Direction, Ray.Distance)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool QP::AABB::CheckCollisionLine(QP::RayVector& Ray) {
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

BoundingBox QP::AABB::Get() const{
	return aabb;
}



void QP::OOBB::Update(float X, float Y, float BoxWidth, float BoxHeight, float RotationValue) {
	oobb.Center = XMFLOAT3(X, Y, 0.0);
	oobb.Extents = XMFLOAT3(BoxWidth * 0.5, BoxHeight * 0.5, 0.0);
	XMVECTOR Quaternion = XMQuaternionRotationRollPitchYaw(0.0, 0.0, XMConvertToRadians(-RotationValue));
	XMStoreFloat4(&oobb.Orientation, Quaternion);
	Rotation = RotationValue;
}

void QP::OOBB::Update(QP::Vector2& Position, float BoxWidth, float BoxHeight, float RotationValue) {
	oobb.Center = XMFLOAT3(Position.x, Position.y, 0.0);
	oobb.Extents = XMFLOAT3(BoxWidth * 0.5, BoxHeight * 0.5, 0.0);
	XMVECTOR Quaternion = XMQuaternionRotationRollPitchYaw(0.0, 0.0, XMConvertToRadians(-RotationValue));
	XMStoreFloat4(&oobb.Orientation, Quaternion);
	Rotation = RotationValue;
}

void QP::OOBB::Render() {
	if (ENABLE_DEV_MODE && ENABLE_RENDER_BOUND_BOX) {
		LineRect.SetColor(1.0, 0.0, 0.0);
		RECT.SetColor(1.0, 0.0, 0.0);

		LineRect.Render(oobb.Center.x, oobb.Center.y, oobb.Extents.x * 2.0, oobb.Extents.y * 2.0, Thickness, Rotation);
		if (Collide)
			RECT.Render(oobb.Center.x, oobb.Center.y, oobb.Extents.x * 2.0, oobb.Extents.y * 2.0, Rotation, 0.3);
	}
}


void QP::OOBB::SetThickness(float Value) {
	if (Value <= 0.0)
		return;
	Thickness = Value;
}

bool QP::OOBB::CheckCollision(const OOBB& Other) {
	if (oobb.Intersects(Other.oobb)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool QP::OOBB::CheckCollision(const AABB& Other) {
	if (oobb.Intersects(Other.Get())) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool QP::OOBB::CheckCollision(const BoundingCircle& Other) {
	if (oobb.Intersects(Other.Get())) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool QP::OOBB::CheckCollisionPoint(float X, float Y) {
	XMFLOAT3 Point(X, Y, 0.0);
	if (oobb.Contains(XMLoadFloat3(&Point)) == ContainmentType::CONTAINS) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool QP::OOBB::CheckCollisionPoint(QP::Vector2& Position) {
	XMFLOAT3 Point(Position.x, Position.y, 0.0);
	if (oobb.Contains(XMLoadFloat3(&Point)) == ContainmentType::CONTAINS) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool QP::OOBB::CheckCollisionRay(QP::RayVector& Ray) {
	if (oobb.Intersects(Ray.Origin, Ray.Direction, Ray.Distance)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}


bool QP::OOBB::CheckCollisionLine(QP::RayVector& Ray) {
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

BoundingOrientedBox QP::OOBB::Get() const {
	return oobb;
}



void QP::BoundingCircle::Update(float X, float Y, float SizeValue) {
	sphere.Center = XMFLOAT3(X, Y, 0.0);
	sphere.Radius = SizeValue * 0.5;
}

void QP::BoundingCircle::Update(QP::Vector2& Position, float SizeValue) {
	sphere.Center = XMFLOAT3(Position.x, Position.y, 0.0);
	sphere.Radius = SizeValue * 0.5;
}

void QP::BoundingCircle::Render() {
	if (ENABLE_DEV_MODE && ENABLE_RENDER_BOUND_BOX) {
		Circle.SetColor(1.0, 0.0, 0.0);
		LineCircle.SetColor(1.0, 0.0, 0.0);

		LineCircle.Draw(sphere.Center.x, sphere.Center.y, sphere.Radius * 2.0 - Thickness, 0.01);
		if (Collide)
			Circle.Render(sphere.Center.x, sphere.Center.y, sphere.Radius * 2.0, 0.3);
	}
}

void QP::BoundingCircle::SetThickness(float Value) {
	if (Value <= 0.0)
		return;
	Thickness = Value;
}

bool QP::BoundingCircle::CheckCollision(const BoundingCircle& Other) {
	if (sphere.Intersects(Other.sphere)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool QP::BoundingCircle::CheckCollision(const AABB& Other) {
	if (sphere.Intersects(Other.Get())) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool QP::BoundingCircle::CheckCollision(const OOBB& Other) {
	if (sphere.Intersects(Other.Get())) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool QP::BoundingCircle::CheckCollisionPoint(float X, float Y) {
	XMFLOAT3 Point(X, Y, 0.0);
	if (sphere.Contains(XMLoadFloat3(&Point)) == ContainmentType::CONTAINS) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool QP::BoundingCircle::CheckCollisionPoint(QP::Vector2& Position) {
	XMFLOAT3 Point(Position.x, Position.y, 0.0);
	if (sphere.Contains(XMLoadFloat3(&Point)) == ContainmentType::CONTAINS) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool QP::BoundingCircle::CheckCollisionRay(QP::RayVector& Ray) {
	if (sphere.Intersects(Ray.Origin, Ray.Direction, Ray.Distance)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool QP::BoundingCircle::CheckCollisionLine(QP::RayVector& Ray) {
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


BoundingSphere QP::BoundingCircle::Get() const {
	return sphere;
}