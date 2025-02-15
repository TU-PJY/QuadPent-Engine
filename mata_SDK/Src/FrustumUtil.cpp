#include "FrustumUtil.h"
#include "Config.h"
#include "CameraUtil.h"
#include "CameraController.h"

SDK_Frustum SDK::Frustum;

void SDK_Frustum::Update() {
	FrustumOOBB.Update(SDK::CameraPosition, ASP(2.0) / SDK::Camera.ZoomValue, 2.0 / SDK::Camera.ZoomValue, SDK::CameraRotation);

if(SHOW_FRUSTUM_BOUND)
	LineRect.Draw(SDK::CameraPosition.x, SDK::CameraPosition.y, ASP(2.0) / SDK::Camera.ZoomValue, 2.0 / SDK::Camera.ZoomValue, BOUND_BOX_THICKNESS, SDK::CameraRotation);
}

bool SDK_Frustum::Check(SDK::AABB& aabb) {
	if (FrustumOOBB.CheckCollision(aabb))
		return true;

	return false;
}

bool SDK_Frustum::Check(SDK::OOBB& oobb) {
	if (FrustumOOBB.CheckCollision(oobb))
		return true;

	return false;
}

bool SDK_Frustum::Check(SDK::BoundingCircle& circle) {
	if (FrustumOOBB.CheckCollision(circle))
		return true;

	return false;
}

SDK_Frustum::SDK_Frustum() {
	LineRect.SetRenderType(RENDER_TYPE_DEFAULT);
	LineRect.SetColor(1.0, 0.0, 0.0);
}