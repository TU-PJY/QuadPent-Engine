#include "SDK_Object.h"
#include "SDK_Camera.h"

void SDK::Object::Begin(int RenderType) {
	SDK::Transform.Identity(LocalMatrix);
	LocalFlipFlag = FLIP_TYPE_NONE;

	LocalOpacityValue = 1.0f;
	LocalBlurValue = 0.0;
	LocalColorValue = SDK::Color3(0.0, 0.0, 0.0);

	SDK::Camera.SetCamera(RenderType);
}

void SDK::Object::ResetGlobalAttribute() {
	SDK::Transform.Identity(GlobalMatrix);
	GlobalFlipFlag = FLIP_TYPE_NONE;

	GlobalOpacityValue = 1.0f;
	GlobalBlurValue = 0.0f;
	GlobalColorValue = SDK::Vector3(0.0, 0.0, 0.0);
}

void SDK::Object::ComputeViewportPosition(float& DestX, float& DestY, bool ApplyAspect) {
	if (ApplyAspect)
		DestX = ASP(ViewportPosition().x);
	else
		DestX = ViewportPosition().x;
	DestY = ViewportPosition().y;
}

void SDK::Object::ComputeViewportPosition(SDK::Vector2& DestValue, bool ApplyAspect) {
	if (ApplyAspect) {
		DestValue.x = ASP(ViewportPosition().x);
		DestValue.y = ViewportPosition().y;
	}
	else
		DestValue = ViewportPosition();
}

void SDK::Object::ComputeLocalPosition(float& DestX, float& DestY) {
	DestX = LocalPosition().x;
	DestY = LocalPosition().y;
}

void SDK::Object::ComputeLocalPosition(SDK::Vector2& DestPosition) {
	DestPosition = LocalPosition();
}

////////////////////////// private

glm::vec4 SDK::Object::ViewportPosition() {
	if (USE_COMPUTE_SHADER)
		SDK::ComputeTool.ComputeMatrix(SDK::ViewportPositionMatrix, SDK::Camera.Projection, SDK::Camera.ViewMatrix, ResultMatrix);
	else
		SDK::ViewportPositionMatrix = SDK::Camera.Projection * SDK::Camera.ViewMatrix * ResultMatrix;

	return SDK::ViewportPositionMatrix * SDK::Vector4(0.0f, 0.0f, 0.0f, 1.0f);
}

glm::vec4 SDK::Object::LocalPosition() {
	return SDK::ResultMatrix * SDK::Vector4(0.0f, 0.0f, 0.0f, 1.0f);
}