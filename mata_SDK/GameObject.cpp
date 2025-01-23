#include "GameObject.h"

void GameObject::BeginRender(int RenderType) {
	transform.Identity(TranslateMatrix);
	transform.Identity(RotateMatrix);
	transform.Identity(ScaleMatrix);
	transform.Identity(ImageAspectMatrix);
	transform.Identity(FlipMatrix);

	ObjectOpacity = 1.0f;
	ObjectBlur = 0.0;

	camera.SetCamera(RenderType);
}

void GameObject::SetColor(GLfloat R, GLfloat G, GLfloat B) {
	ObjectColor.r = R;
	ObjectColor.g = G;
	ObjectColor.b = B;
}

void GameObject::SetColor(glm::vec3& Color) {
	ObjectColor.r = Color.r;
	ObjectColor.g = Color.g;
	ObjectColor.b = Color.b;
}

void GameObject::SetColorRGB(int R, int G, int B) {
	ObjectColor.r = (1.0f / 255.0f) * (GLfloat)R;
	ObjectColor.g = (1.0f / 255.0f) * (GLfloat)G;
	ObjectColor.b = (1.0f / 255.0f) * (GLfloat)B;
}

void GameObject::ComputeViewportPosition(GLfloat& DestX, GLfloat& DestY, bool ApplyAspect) {
	if (ApplyAspect)
		DestX = ASP(ViewportPosition().x);
	else
		DestX = ViewportPosition().x;
	DestY = ViewportPosition().y;
}

void GameObject::ComputeViewportPosition(glm::vec2& DestValue, bool ApplyAspect) {
	if (ApplyAspect) {
		DestValue.x = ASP(ViewportPosition().x);
		DestValue.y = ViewportPosition().y;
	}
	else
		DestValue = ViewportPosition();
}

void GameObject::ComputeLocalPosition(GLfloat& DestX, GLfloat& DestY) {
	DestX = LocalPosition().x;
	DestY = LocalPosition().y;
}

void GameObject::ComputeLocalPosition(glm::vec2& DestPosition) {
	DestPosition = LocalPosition();
}

void GameObject::SetFlip(int FlipOpt) {
	switch (FlipOpt) {
	case FLIP_TYPE_NONE:
		transform.Identity(FlipMatrix);
		break;

	case FLIP_TYPE_X:
		transform.RotateH(FlipMatrix, 180.0f);
		break;

	case FLIP_TYPE_Y:
		transform.RotateV(FlipMatrix, 180.0f);
		break;

	case FLIP_TYPE_XY:
		transform.RotateH(FlipMatrix, 180.0f);
		transform.RotateV(FlipMatrix, 180.0f);
		break;
	}
}

void GameObject::SetBlur(GLfloat Strength) {
	ObjectBlur = Strength;
}

void GameObject::SetUnitFlip(int FlipOpt) {
	switch (FlipOpt) {
	case FLIP_TYPE_NONE:
		transform.Identity(UnitFlipMatrix);
		break;

	case FLIP_TYPE_X:
		transform.RotateH(UnitFlipMatrix, 180.0f);
		break;

	case FLIP_TYPE_Y:
		transform.RotateV(UnitFlipMatrix, 180.0f);
		break;

	case FLIP_TYPE_XY:
		transform.RotateH(UnitFlipMatrix, 180.0f);
		transform.RotateV(UnitFlipMatrix, 180.0f);
		break;
	}
}

void GameObject::SetUnitOpacity(GLfloat Value) {
	UnitOpacity = Value;
}

void GameObject::SetUnitBlur(GLfloat Strength) {
	UnitBlur = Strength;
}

void GameObject::ResetUnitTransform() {
	transform.Identity(UnitTranslateMatrix);
	transform.Identity(UnitRotateMatrix);
	transform.Identity(UnitScaleMatrix);
	transform.Identity(UnitFlipMatrix);
	UnitOpacity = 1.0f;
	UnitBlur = 0.0f;
}

void GameObject::RenderImage(Image& Image, GLfloat OpacityValue, bool ApplyUnitTransform, bool DisableAdjustAspect) {
	if (!DisableAdjustAspect)
		transform.ImageScale(ImageAspectMatrix, Image.Width, Image.Height);

	computeUtil.ComputeMatrix(ResultMatrix, TranslateMatrix, RotateMatrix, ScaleMatrix, ImageAspectMatrix, FlipMatrix);
	ObjectOpacity = OpacityValue;

	if (ApplyUnitTransform) {
		computeUtil.ComputeMatrix(ResultMatrix, UnitTranslateMatrix, UnitRotateMatrix, UnitScaleMatrix, UnitFlipMatrix, ResultMatrix);
		ObjectOpacity -= (1.0f - UnitOpacity);
		EX.ClampValue(ObjectOpacity, 0.0, CLAMP_LESS);
		ObjectBlur += UnitBlur;
	}

	PrepareRender(Image);
	imageUtil.Render(Image);
}

void GameObject::RenderSpriteSheet(SpriteSheet& SpriteSheetStruct, GLfloat OpacityValue, GLfloat& Frame, bool ApplyUnitTransform, bool DisableAdjustAspect) {
	if ((int)Frame >= SpriteSheetStruct.Frame)
		Frame = 0.0;

	if (!DisableAdjustAspect)
		transform.ImageScale(ImageAspectMatrix, SpriteSheetStruct.Width, SpriteSheetStruct.Height);

	computeUtil.ComputeMatrix(ResultMatrix, TranslateMatrix, RotateMatrix, ScaleMatrix, ImageAspectMatrix, FlipMatrix);
	ObjectOpacity = OpacityValue;

	if (ApplyUnitTransform) {
		computeUtil.ComputeMatrix(ResultMatrix, UnitTranslateMatrix, UnitRotateMatrix, UnitScaleMatrix, UnitFlipMatrix, ResultMatrix);
		ObjectOpacity -= (1.0f - UnitOpacity);
		EX.ClampValue(ObjectOpacity, 0.0, CLAMP_LESS);
		ObjectBlur += UnitBlur;
	}

	PrepareRender(SpriteSheetStruct);
	imageUtil.RenderSheet(SpriteSheetStruct, (int)Frame);
}

////////////////////////// private

void GameObject::PrepareRender(Image& ImageStruct) {
	glUseProgram(IMAGE_SHADER);
	camera.PrepareRender(SHADER_TYPE_IMAGE);

	glUniform1f(IMAGE_OPACITY_LOCATION, ObjectOpacity);
	glUniform3f(IMAGE_COLOR_LOCATION, ObjectColor.r, ObjectColor.g, ObjectColor.b);

	if (ObjectBlur > 0.0) {
		glUniform1i(BLUR_STATE_LOCATION, 1);
		glUniform1f(BLUR_STRENGTH_LOCATION, ObjectBlur);
		glUniform2f(TEXTURE_SIZE_LOCATION, 1.0 / (GLfloat)ImageStruct.Width, 1.0 / (GLfloat)ImageStruct.Height);
	}
	else  
		glUniform1i(BLUR_STATE_LOCATION, 0);

	glUniformMatrix4fv(IMAGE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(ResultMatrix));
}

void GameObject::PrepareRender(SpriteSheet& SpriteSheetStruct) {
	glUseProgram(IMAGE_SHADER);
	camera.PrepareRender(SHADER_TYPE_IMAGE);

	glUniform1f(IMAGE_OPACITY_LOCATION, ObjectOpacity);
	glUniform3f(IMAGE_COLOR_LOCATION, ObjectColor.r, ObjectColor.g, ObjectColor.b);

	if (ObjectBlur > 0.0) {
		glUniform1i(BLUR_STATE_LOCATION, 1);
		glUniform1f(BLUR_STRENGTH_LOCATION, ObjectBlur);
		glUniform2f(TEXTURE_SIZE_LOCATION, 1.0 / (GLfloat)SpriteSheetStruct.Width, 1.0 / (GLfloat)SpriteSheetStruct.Height);
	}
	else
		glUniform1i(BLUR_STATE_LOCATION, 0);

	glUniformMatrix4fv(IMAGE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(ResultMatrix));
}

glm::vec4 GameObject::ViewportPosition() {
	computeUtil.ComputeMatrix(ViewportPositionMatrix, camera.Projection, camera.ViewMatrix, ResultMatrix);
	return ViewportPositionMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

glm::vec4 GameObject::LocalPosition() {
	return ResultMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}