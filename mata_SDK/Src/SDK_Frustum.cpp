#include "SDK_Frustum.h"
#include "SDK_Config.h"
#include "SDK_Camera.h"
#include "SDK_CameraController.h"

MSDK::SDK_Frustum MSDK::Frustum;

void MSDK::SDK_Frustum::Update() {
	FrustumOOBB.Update(MSDK::Camera.Position, (2.0 * MSDK::ASPECT) / MSDK::Camera.Zoom, 2.0 / MSDK::Camera.Zoom, MSDK::Camera.Rotation);

if(SHOW_FRUSTUM_BOUND)
	LineRect.Draw(MSDK::Camera.Position.x, MSDK::Camera.Position.y, (2.0 * MSDK::ASPECT) / MSDK::Camera.Zoom, 2.0 / MSDK::Camera.Zoom, BOUND_BOX_THICKNESS, MSDK::Camera.Rotation);
}

bool MSDK::SDK_Frustum::Check(MSDK::AABB& aabb) {
	if (FrustumOOBB.CheckCollision(aabb))
		return true;

	return false;
}

bool MSDK::SDK_Frustum::Check(MSDK::OOBB& oobb) {
	if (FrustumOOBB.CheckCollision(oobb))
		return true;

	return false;
}

bool MSDK::SDK_Frustum::Check(MSDK::BoundingCircle& circle) {
	if (FrustumOOBB.CheckCollision(circle))
		return true;

	return false;
}

MSDK::SDK_Frustum::SDK_Frustum() {
	LineRect.SetRenderType(RENDER_TYPE_DEFAULT);
	LineRect.SetColor(1.0, 0.0, 0.0);
}