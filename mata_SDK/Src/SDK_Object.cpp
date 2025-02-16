#include "SDK_Object.h"
#include "SDK_Camera.h"

void SDK::Object::Begin(int RenderType) {
	SDK::Transform.Identity(MoveMatrix);
	SDK::Transform.Identity(RotateMatrix);
	SDK::Transform.Identity(ScaleMatrix);
	SDK::Transform.Identity(FlipMatrix);

	ObjectOpacityValue = 1.0f;
	ObjectBlurValue = 0.0;
	ObjectColor = glm::vec3(0.0, 0.0, 0.0);

	SDK::Camera.SetCamera(RenderType);
}

void SDK::Object::IdentityUnitMatrix() {
	SDK::Transform.Identity(UnitMoveMatrix);
	SDK::Transform.Identity(UnitRotateMatrix);
	SDK::Transform.Identity(UnitScaleMatrix);
	SDK::Transform.Identity(UnitFlipMatrix);

	UnitOpacityValue = 1.0f;
	UnitBlurValue = 0.0f;
}

void SDK::Object::SetColor(GLfloat R, GLfloat G, GLfloat B) {
	ObjectColor.r = R;
	ObjectColor.g = G;
	ObjectColor.b = B;
}

void SDK::Object::SetColor(glm::vec3 Color) {
	ObjectColor = Color;
}

void SDK::Object::SetColorRGB(int R, int G, int B) {
	ObjectColor.r = (1.0f / 255.0f) * (GLfloat)R;
	ObjectColor.g = (1.0f / 255.0f) * (GLfloat)G;
	ObjectColor.b = (1.0f / 255.0f) * (GLfloat)B;
}

void SDK::Object::ComputeViewportPosition(GLfloat& DestX, GLfloat& DestY, bool ApplyAspect) {
	if (ApplyAspect)
		DestX = ASP(ViewportPosition().x);
	else
		DestX = ViewportPosition().x;
	DestY = ViewportPosition().y;
}

void SDK::Object::ComputeViewportPosition(glm::vec2& DestValue, bool ApplyAspect) {
	if (ApplyAspect) {
		DestValue.x = ASP(ViewportPosition().x);
		DestValue.y = ViewportPosition().y;
	}
	else
		DestValue = ViewportPosition();
}

void SDK::Object::ComputeLocalPosition(GLfloat& DestX, GLfloat& DestY) {
	DestX = LocalPosition().x;
	DestY = LocalPosition().y;
}

void SDK::Object::ComputeLocalPosition(glm::vec2& DestPosition) {
	DestPosition = LocalPosition();
}

////////////////////////// private

glm::vec4 SDK::Object::ViewportPosition() {
	SDK::ComputeTool.ComputeMatrix(SDK::ViewportPositionMatrix, SDK::Camera.Projection, SDK::Camera.ViewMatrix, ResultMatrix);
	return SDK::ViewportPositionMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

glm::vec4 SDK::Object::LocalPosition() {
	return SDK::ResultMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}