#include "SDK_Object.h"
#include "SDK_Camera.h"

void MSDK::Object::Begin(int RenderType) {
	MSDK::Transform.Identity(MoveMatrix);
	MSDK::Transform.Identity(RotateMatrix);
	MSDK::Transform.Identity(ScaleMatrix);
	MSDK::Transform.Identity(FlipMatrix);

	ObjectOpacityValue = 1.0f;
	ObjectBlurValue = 0.0;
	ObjectColor = glm::vec3(0.0, 0.0, 0.0);

	MSDK::Camera.SetCamera(RenderType);
}

void MSDK::Object::IdentityUnitMatrix() {
	MSDK::Transform.Identity(UnitMoveMatrix);
	MSDK::Transform.Identity(UnitRotateMatrix);
	MSDK::Transform.Identity(UnitScaleMatrix);
	MSDK::Transform.Identity(UnitFlipMatrix);

	UnitOpacityValue = 1.0f;
	UnitBlurValue = 0.0f;
}

void MSDK::Object::SetColor(float R, float G, float B) {
	ObjectColor.r = R;
	ObjectColor.g = G;
	ObjectColor.b = B;
}

void MSDK::Object::SetColor(glm::vec3 Color) {
	ObjectColor = Color;
}

void MSDK::Object::SetColorRGB(int R, int G, int B) {
	ObjectColor.r = (1.0f / 255.0f) * (float)R;
	ObjectColor.g = (1.0f / 255.0f) * (float)G;
	ObjectColor.b = (1.0f / 255.0f) * (float)B;
}

void MSDK::Object::ComputeViewportPosition(float& DestX, float& DestY, bool ApplyAspect) {
	if (ApplyAspect)
		DestX = ASP(ViewportPosition().x);
	else
		DestX = ViewportPosition().x;
	DestY = ViewportPosition().y;
}

void MSDK::Object::ComputeViewportPosition(glm::vec2& DestValue, bool ApplyAspect) {
	if (ApplyAspect) {
		DestValue.x = ASP(ViewportPosition().x);
		DestValue.y = ViewportPosition().y;
	}
	else
		DestValue = ViewportPosition();
}

void MSDK::Object::ComputeLocalPosition(float& DestX, float& DestY) {
	DestX = LocalPosition().x;
	DestY = LocalPosition().y;
}

void MSDK::Object::ComputeLocalPosition(glm::vec2& DestPosition) {
	DestPosition = LocalPosition();
}

////////////////////////// private

glm::vec4 MSDK::Object::ViewportPosition() {
	MSDK::ComputeTool.ComputeMatrix(MSDK::ViewportPositionMatrix, MSDK::Camera.Projection, MSDK::Camera.ViewMatrix, ResultMatrix);
	return MSDK::ViewportPositionMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

glm::vec4 MSDK::Object::LocalPosition() {
	return MSDK::ResultMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}