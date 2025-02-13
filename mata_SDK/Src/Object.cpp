#include "Object.h"
#include "CameraUtil.h"

void Object::Begin(int RenderType) {
	transform.Identity(ResultMatrix);
	transform.Identity(MoveMatrix);
	transform.Identity(RotateMatrix);
	transform.Identity(ScaleMatrix);
	transform.Identity(ImageAspectMatrix);
	transform.Identity(FlipMatrix);

	ObjectOpacityValue = 1.0f;
	ObjectBlurValue = 0.0;
	ObjectColor = glm::vec3(0.0, 0.0, 0.0);

	camera.SetCamera(RenderType);
}

void Object::IdentityUnitMatrix() {
	transform.Identity(UnitMoveMatrix);
	transform.Identity(UnitRotateMatrix);
	transform.Identity(UnitScaleMatrix);
	transform.Identity(UnitFlipMatrix);

	UnitOpacityValue = 1.0f;
	UnitBlurValue = 0.0f;
}

void Object::ComputeViewportPosition(GLfloat& DestX, GLfloat& DestY, bool ApplyAspect) {
	if (ApplyAspect)
		DestX = ASP(ViewportPosition().x);
	else
		DestX = ViewportPosition().x;
	DestY = ViewportPosition().y;
}

void Object::ComputeViewportPosition(glm::vec2& DestValue, bool ApplyAspect) {
	if (ApplyAspect) {
		DestValue.x = ASP(ViewportPosition().x);
		DestValue.y = ViewportPosition().y;
	}
	else
		DestValue = ViewportPosition();
}

void Object::ComputeLocalPosition(GLfloat& DestX, GLfloat& DestY) {
	DestX = LocalPosition().x;
	DestY = LocalPosition().y;
}

void Object::ComputeLocalPosition(glm::vec2& DestPosition) {
	DestPosition = LocalPosition();
}

////////////////////////// private

glm::vec4 Object::ViewportPosition() {
	computeUtil.ComputeMatrix(ViewportPositionMatrix, camera.Projection, camera.ViewMatrix, ResultMatrix);
	return ViewportPositionMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

glm::vec4 Object::LocalPosition() {
	return ResultMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}