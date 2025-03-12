#include "QuadPent_ObjectFunction.h"
#include "QuadPent_Camera.h"
#include "QuadPent_ObjectValue.h"
#include "QuadPent_Transform.h"

void QP::Begin(int RenderType) {
	QP::Transform.Identity(LocalMatrix);
	LocalFlipFlag = FLIP_TYPE_NONE;

	LocalOpacityValue = 1.0f;
	LocalBlurValue = 0.0;
	LocalColorValue = QP::Color3(0.0, 0.0, 0.0);

	QP::Camera.SetCamera(RenderType);
}

void QP::ResetGlobalAttribute() {
	QP::Transform.Identity(GlobalMatrix);
	GlobalFlipFlag = FLIP_TYPE_NONE;

	GlobalOpacityValue = 1.0f;
	GlobalBlurValue = 0.0f;
	GlobalColorValue = QP::Color3(0.0, 0.0, 0.0);
}

void QP::GetViewportPosition(QP::Vector2& DestPosition) {
	QP::ViewportPositionMatrix = QP::Camera.Projection * QP::Camera.ViewMatrix * ResultMatrix;
	QP::Vector4 ViewportPositionVec = QP::ViewportPositionMatrix * QP::Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	DestPosition.x = ViewportPositionVec.x * Aspect;
	DestPosition.y = ViewportPositionVec.y;
}

void QP::GetViewportPosition(float& DestX, float& DestY) {
	QP::ViewportPositionMatrix = QP::Camera.Projection * QP::Camera.ViewMatrix * ResultMatrix;
	QP::Vector4 ViewportPositionVec = QP::ViewportPositionMatrix * QP::Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	DestX = ViewportPositionVec.x * Aspect;
	DestY = ViewportPositionVec.y;
}

void QP::GetLocalPosition(QP::Vector2& DestPosition) {
	QP::Vector4 LocalPositionVec = QP::ResultMatrix * QP::Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	DestPosition.x = LocalPositionVec.x;
	DestPosition.y = LocalPositionVec.y;
}

void QP::GetLocalPosition(float& DestX, float& DestY) {
	QP::Vector4 LocalPositionVec = QP::ResultMatrix * QP::Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	DestX = LocalPositionVec.x;
	DestY = LocalPositionVec.y;
}