#include "SDK_ObjectFunction.h"
#include "SDK_Camera.h"
#include "SDK_ObjectValue.h"
#include "SDK_Transform.h"
#include "SDK_ComputeTool.h"

void SDK::Begin(int RenderType) {
	SDK::Transform.Identity(LocalMatrix);
	LocalFlipFlag = FLIP_TYPE_NONE;

	LocalOpacityValue = 1.0f;
	LocalBlurValue = 0.0;
	LocalColorValue = SDK::Color3(0.0, 0.0, 0.0);

	SDK::Camera.SetCamera(RenderType);
}

void SDK::ResetGlobalAttribute() {
	SDK::Transform.Identity(GlobalMatrix);
	GlobalFlipFlag = FLIP_TYPE_NONE;

	GlobalOpacityValue = 1.0f;
	GlobalBlurValue = 0.0f;
	GlobalColorValue = SDK::Color3(0.0, 0.0, 0.0);
}

void SDK::GetViewportPosition(SDK::Vector2& DestPosition) {
	SDK::ViewportPositionMatrix = SDK::Camera.Projection * SDK::Camera.ViewMatrix * ResultMatrix;
	SDK::Vector4 ViewportPositionVec = SDK::ViewportPositionMatrix * SDK::Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	DestPosition.x = ViewportPositionVec.x * Aspect;
	DestPosition.y = ViewportPositionVec.y;
}

void SDK::GetViewportPosition(float& DestX, float& DestY) {
	SDK::ViewportPositionMatrix = SDK::Camera.Projection * SDK::Camera.ViewMatrix * ResultMatrix;
	SDK::Vector4 ViewportPositionVec = SDK::ViewportPositionMatrix * SDK::Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	DestX = ViewportPositionVec.x * Aspect;
	DestY = ViewportPositionVec.y;
}

void SDK::GetLocalPosition(SDK::Vector2& DestPosition) {
	SDK::Vector4 LocalPositionVec = SDK::ResultMatrix * SDK::Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	DestPosition.x = LocalPositionVec.x;
	DestPosition.y = LocalPositionVec.y;
}

void SDK::GetLocalPosition(float& DestX, float& DestY) {
	SDK::Vector4 LocalPositionVec = SDK::ResultMatrix * SDK::Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	DestX = LocalPositionVec.x;
	DestY = LocalPositionVec.y;
}