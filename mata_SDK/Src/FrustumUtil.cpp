#include "FrustumUtil.h"
#include "Config.h"
#include "CameraUtil.h"
#include "CameraController.h"

FrustumUtil frustum;

void FrustumUtil::Update() {
	FrustumOOBB.Update(CameraPosition, ASP(2.0) / camera.ZoomValue, 2.0 / camera.ZoomValue, CameraRotation);

if(SHOW_FRUSTUM_BOUND)
	LineRect.Draw(0.0, 0.0, ASP(2.0), 2.0, 0.01, 0.0);
}

bool FrustumUtil::Check(AABB& aabb) {
	if (FrustumOOBB.CheckCollision(aabb))
		return true;

	return false;
}

bool FrustumUtil::Check(OOBB& oobb) {
	if (FrustumOOBB.CheckCollision(oobb))
		return true;

	return false;
}

bool FrustumUtil::Check(BoundingCircle& circle) {
	if (FrustumOOBB.CheckCollision(circle))
		return true;

	return false;
}

FrustumUtil::FrustumUtil() {
	LineRect.SetRenderType(RENDER_TYPE_STATIC);
	LineRect.SetColor(1.0, 0.0, 0.0);
}