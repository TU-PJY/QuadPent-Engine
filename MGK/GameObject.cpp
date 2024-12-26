#include "GameObject.h"

void GameObject::InitRenderState(int RenderType) {
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

void GameObject::UpdateViewportPosition(GLfloat& DestX, GLfloat& DestY, bool ApplyAspect) {
	if (ApplyAspect)
		DestX = ASP(ViewportPosition().x);
	else
		DestX = ViewportPosition().x;
	DestY = ViewportPosition().y;
}

void GameObject::UpdateViewportPosition(glm::vec2& DestValue, bool ApplyAspect) {
	if (ApplyAspect) {
		DestValue.x = ASP(ViewportPosition().x);
		DestValue.y = ViewportPosition().y;
	}
	else
		DestValue = ViewportPosition();
}

void GameObject::UpdateLocalPosition(GLfloat& DestX, GLfloat& DestY) {
	DestX = LocalPosition().x;
	DestY = LocalPosition().y;
}

void GameObject::UpdateLocalPosition(glm::vec2& DestPosition) {
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

void GameObject::Render(Image& Image, GLfloat OpacityValue, bool ApplyUnitTransform, bool DisableAdjustAspect) {
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

void GameObject::DrawImage(int RenderType, Image& Image, GLfloat X, GLfloat Y, GLfloat Width, GLfloat Height, GLfloat Rotation, GLfloat OpacityValue, int FlipOpt, bool ApplyUnitTransform, bool DisableAdjustAspect) {
	InitRenderState(RenderType);
	transform.Move(TranslateMatrix, X, Y);
	transform.Rotate(RotateMatrix, Rotation);
	transform.Scale(ScaleMatrix, Width, Height);
	SetFlip(FlipOpt);
	ObjectOpacity = OpacityValue;
	Render(Image, OpacityValue, ApplyUnitTransform, DisableAdjustAspect);
}

void GameObject::DrawImage(int RenderType, Image& Image, glm::vec2& Position, GLfloat Width, GLfloat Height, GLfloat Rotation, GLfloat OpacityValue, int FlipOpt, bool ApplyUnitTransform, bool DisableAdjustAspect) {
	InitRenderState(RenderType);
	transform.Move(TranslateMatrix, Position);
	transform.Rotate(RotateMatrix, Rotation);
	transform.Scale(ScaleMatrix, Width, Height);
	SetFlip(FlipOpt);
	ObjectOpacity = OpacityValue;
	Render(Image, OpacityValue, ApplyUnitTransform, DisableAdjustAspect);
}

#ifdef USE_SOUND_SYSTEM
void GameObject::PlaySound(Sound Sound, SoundChannel& ChannelVar, unsigned int StartTime) {
	soundUtil.PlaySound(Sound, ChannelVar, StartTime);
}

void GameObject::PlaySoundOnce(Sound Sound, SoundChannel& ChannelVar, bool& BoolValue, unsigned int StartTime) {
	if (BoolValue) {
		soundUtil.PlaySound(Sound, ChannelVar, StartTime);
		BoolValue = false;
	}
}

void GameObject::PauseSound(SoundChannel& ChannelVar, bool Flag) {
	soundUtil.PauseSound(ChannelVar, Flag);
}

void GameObject::StopSound(SoundChannel& ChannelVar) {
	soundUtil.StopSound(ChannelVar);
}

void GameObject::SetPlaySpeed(SoundChannel& ChannelVar, float PlaySpeed) {
	soundUtil.SetPlaySpeed(ChannelVar, PlaySpeed);
}

void GameObject::ResetPlaySpeed(SoundChannel& ChannelVar) {
	soundUtil.ResetPlaySpeed(ChannelVar);
}

void GameObject::EnableFreqCutoff(SoundChannel& ChannelVar, float Frequency) {
	soundUtil.SetFreqCutOff(ChannelVar, Frequency);
}

void GameObject::EnableBeatDetect(SoundChannel& ChannelVar) {
	soundUtil.SetBeatDetect(ChannelVar);
}

void GameObject::DetectBeat(GLfloat& Value, float ThresHold, float SamplingRate) {
	Value =  soundUtil.DetectBeat(ThresHold, SamplingRate);
}

GLfloat GameObject::DetectBeat(float ThresHold, float SamplingRate) {
	return soundUtil.DetectBeat(ThresHold, SamplingRate);
}

bool GameObject::IsBeat(float ThresHold, float SamplingRate) {
	return soundUtil.IsBeat(ThresHold, SamplingRate);
}

void GameObject::DisableFreqCutoff(SoundChannel& ChannelVar) {
	soundUtil.UnSetFreqCutOff(ChannelVar);
}

void GameObject::DisableBeatDetect(SoundChannel& ChannelVar) {
	soundUtil.UnSetBeatDetect(ChannelVar);
}

void GameObject::SetSoundDistance(SoundChannel& ChannelVar, float MinDist, float MaxDist) {
	soundUtil.SetDistance(ChannelVar, MinDist, MaxDist);
}

void GameObject::SetListnerPosition(float X, float Y) {
	soundUtil.SetListnerPosition(X, Y);
}

void GameObject::SetListnerPosition(glm::vec2& Position) {
	soundUtil.SetListnerPosition(Position.x, Position.y);
}

void GameObject::SetSoundPosition(SoundChannel& ChannelVar, float X, float Y, float Diff) {
	soundUtil.SetSoundPosition(ChannelVar, X, Y, Diff);
}

void GameObject::SetSoundPosition(SoundChannel& ChannelVar, glm::vec2 Position, float Diff) {
	soundUtil.SetSoundPosition(ChannelVar, Position.x, Position.y, Diff);
}
#endif

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

glm::vec4 GameObject::ViewportPosition() {
	computeUtil.ComputeMatrix(ViewportPositionMatrix, camera.Projection, camera.ViewMatrix, ResultMatrix);
	return ViewportPositionMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

glm::vec4 GameObject::LocalPosition() {
	return ResultMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}