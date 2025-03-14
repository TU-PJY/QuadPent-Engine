#include "QuadPent_Frustum.h"
#include "QuadPent_Config.h"
#include "QuadPent_Camera.h"
#include "QuadPent_CameraController.h"

QP::QuadPent_Frustum QP::Frustum;

void QP::QuadPent_Frustum::Update() {
	FrustumOOBB.Update(QP::Camera.Position, ViewportWidth / QP::Camera.Zoom, 2.0 / QP::Camera.Zoom, QP::Camera.Rotation);
}

void QP::QuadPent_Frustum::Render() {
	if (ENABLE_RENDER_FRUSTUM_BOUND)
		LineRect.Render(QP::Camera.Position.x, QP::Camera.Position.y, ViewportWidth / QP::Camera.Zoom, 2.0 / QP::Camera.Zoom, BOUND_BOX_THICKNESS, QP::Camera.Rotation);
}

bool QP::QuadPent_Frustum::Check(QP::AABB& aabb) {
	if (FrustumOOBB.CheckCollision(aabb))
		return true;

	return false;
}

bool QP::QuadPent_Frustum::Check(QP::OOBB& oobb) {
	if (FrustumOOBB.CheckCollision(oobb))
		return true;

	return false;
}

bool QP::QuadPent_Frustum::Check(QP::BoundingCircle& circle) {
	if (FrustumOOBB.CheckCollision(circle))
		return true;

	return false;
}

QP::QuadPent_Frustum::QuadPent_Frustum() {
	LineRect.SetRenderType(RENDER_TYPE_DEFAULT);
	LineRect.SetColor(1.0, 0.0, 0.0);
}