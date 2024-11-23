#include "GameObject.h"
#include "ImageUtil.h"
#include "CameraUtil.h"
#include "Scene.h"
#include <cmath>

void GameObject::InitRenderState(int RenderType) {
	Transform::Identity(TranslateMatrix);
	Transform::Identity(RotateMatrix);
	Transform::Identity(ScaleMatrix);
	Transform::Identity(ImageAspectMatrix);
	Transform::Identity(FlipMatrix);

	TransparencyValue = 1.0f;
	BlurValue = 0.0;

	camera.SetCamera(RenderType);
}

void GameObject::SetColor(GLfloat R, GLfloat G, GLfloat B) {
	ObjectColor.r = R;
	ObjectColor.g = G;
	ObjectColor.b = B;
}

void GameObject::SetColor(glm::vec3 Color) {
	ObjectColor.r = Color.r;
	ObjectColor.g = Color.g;
	ObjectColor.b = Color.b;
}

void GameObject::SetColorRGB(int R, int G, int B) {
	ObjectColor.r = (1.0f / 255.0f) * (GLfloat)R;
	ObjectColor.g = (1.0f / 255.0f) * (GLfloat)G;
	ObjectColor.b = (1.0f / 255.0f) * (GLfloat)B;
}

GLfloat GameObject::ASP(GLfloat Value) {
	return Value * ASPECT;
}

void GameObject::UpdateViewportPosition(GLfloat& ValueX, GLfloat& ValueY, bool ApplyAspect) {
	if (ApplyAspect)
		ValueX = ASP(ViewportPosition().x);
	else
		ValueX = ViewportPosition().x;
	ValueY = ViewportPosition().y;
}

void GameObject::UpdateViewportPosition(glm::vec2& Position, bool ApplyAspect) {
	if (ApplyAspect) {
		Position.x = ASP(ViewportPosition().x);
		Position.y = ViewportPosition().y;
	}
	else
		Position = ViewportPosition();
}

void GameObject::UpdateLocalPosition(GLfloat& ValueX, GLfloat& ValueY) {
	ValueX = LocalPosition().x;
	ValueY = LocalPosition().y;
}

void GameObject::UpdateLocalPosition(glm::vec2& Position) {
	Position = LocalPosition();
}

void GameObject::Flip(int FlipOpt) {
	switch (FlipOpt) {
	case FLIP_TYPE_NONE:
		Transform::Identity(FlipMatrix); 
		break;

	case FLIP_TYPE_X:
		Transform::RotateX(FlipMatrix, 180.0f);
		break;

	case FLIP_TYPE_Y:
		Transform::RotateY(FlipMatrix, 180.0f);
		break;

	case FLIP_TYPE_XY:
		Transform::RotateX(FlipMatrix, 180.0f);
		Transform::RotateY(FlipMatrix, 180.0f);
		break;
	}
}

void GameObject::Blur(int Strength) {
	BlurValue = (GLfloat)Strength;
}

void GameObject::UnitFlip(int FlipOpt) {
	switch (FlipOpt) {
	case FLIP_TYPE_NONE:
		Transform::Identity(UnitFlipMatrix);
		break;

	case FLIP_TYPE_X:
		Transform::RotateX(UnitFlipMatrix, 180.0f);
		break;

	case FLIP_TYPE_Y:
		Transform::RotateY(UnitFlipMatrix, 180.0f);
		break;

	case FLIP_TYPE_XY:
		Transform::RotateX(UnitFlipMatrix, 180.0f);
		Transform::RotateY(UnitFlipMatrix, 180.0f);
		break;
	}
}

void GameObject::UnitTransparent(GLfloat Value) {
	UnitTransparencyValue = Value;
}

void GameObject::UnitBlur(int Value) {
	UnitBlurValue = GLfloat(Value);
}

void GameObject::ResetUnitTransform() {
	Transform::Identity(UnitTranslateMatrix);
	Transform::Identity(UnitRotateMatrix);
	Transform::Identity(UnitScaleMatrix);
	Transform::Identity(UnitFlipMatrix);
	UnitTransparencyValue = 1.0f;
	UnitBlurValue = 0.0f;
}

void GameObject::Render(Image& Image, GLfloat Transparency, bool ApplyUnitTransform, bool DisableAdjustAspect) {
	if (!DisableAdjustAspect)
		Transform::ImageScale(ImageAspectMatrix, Image.Width, Image.Height);

		Compt::ComputeMatrix(ResultMatrix, TranslateMatrix, RotateMatrix, ScaleMatrix, ImageAspectMatrix, FlipMatrix);
		TransparencyValue = Transparency;

	if (ApplyUnitTransform) {
		Compt::ComputeMatrix(ResultMatrix, UnitTranslateMatrix, UnitRotateMatrix, UnitScaleMatrix, UnitFlipMatrix, ResultMatrix);
		TransparencyValue -= (1.0f - UnitTransparencyValue);
		EX::ClampValue(TransparencyValue, 0.0, CLAMP_LESS);
		BlurValue += UnitBlurValue;
	}

	PrepareRender();
	imageUtil.Render(Image);
}

void GameObject::RenderImage(int RenderType, Image& Image, GLfloat X, GLfloat Y, GLfloat Width, GLfloat Height, GLfloat Rotation, GLfloat Transparency, int FlipOpt, bool ApplyUnitTransform, bool DisableAdjustAspect) {
	InitRenderState(RenderType);
	Transform::Move(TranslateMatrix, X, Y);
	Transform::Rotate(RotateMatrix, Rotation);
	Transform::Scale(ScaleMatrix, Width, Height);
	Flip(FlipOpt);
	TransparencyValue = Transparency;
	Render(Image, Transparency, ApplyUnitTransform, DisableAdjustAspect);
}

void GameObject::RenderImage(int RenderType, Image& Image, glm::vec2 Position, GLfloat Width, GLfloat Height, GLfloat Rotation, GLfloat Transparency, int FlipOpt, bool ApplyUnitTransform, bool DisableAdjustAspect) {
	InitRenderState(RenderType);
	Transform::Move(TranslateMatrix, Position);
	Transform::Rotate(RotateMatrix, Rotation);
	Transform::Scale(ScaleMatrix, Width, Height);
	Flip(FlipOpt);
	TransparencyValue = Transparency;
	Render(Image, Transparency, ApplyUnitTransform, DisableAdjustAspect);
}

void GameObject::PlaySound(Sound Sound, SoundChannel& ChannelVar, unsigned int StartTime) {
	soundUtil.PlaySound(Sound, ChannelVar, StartTime);
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

void GameObject::SetListnerPosition(glm::vec2 Position) {
	soundUtil.SetListnerPosition(Position.x, Position.y);
}

void GameObject::SetSoundPosition(SoundChannel& ChannelVar, float X, float Y, float Diff) {
	soundUtil.SetSoundPosition(ChannelVar, X, Y, Diff);
}

void GameObject::SetSoundPosition(SoundChannel& ChannelVar, glm::vec2 Position, float Diff) {
	soundUtil.SetSoundPosition(ChannelVar, Position.x, Position.y, Diff);
}

void GameObject::CheckDeleteReserveCommand() {
	if (DeleteReserveCommand)
		scene.DeleteObject(this);
}

////////////////////////// private

void GameObject::PrepareRender() {
	glUseProgram(IMAGE_SHADER);
	camera.PrepareRender(SHADER_TYPE_IMAGE);

	glUniform1f(IMAGE_ALPHA_LOCATION, TransparencyValue);
	glUniform3f(IMAGE_COLOR_LOCATION, ObjectColor.r, ObjectColor.g, ObjectColor.b);

	if (BlurValue > 0.0) {
		glUniform1i(BOOL_BLUR_LOCATION, 1);
		glUniform1f(BLUR_STRENGTH_LOCATION, BlurValue);
		glUniform2f(TEXEL_SIZE_LOCATION, ASP(1.0) / (GLfloat)WIDTH, 1.0 / (GLfloat)HEIGHT);
	}
	else  
		glUniform1i(BOOL_BLUR_LOCATION, 0);

	glUniformMatrix4fv(IMAGE_MODEL_LOCATION, 1, GL_FALSE, value_ptr(ResultMatrix));
}

glm::vec4 GameObject::ViewportPosition() {
	Compt::ComputeMatrix(ViewportPositionMatrix, camera.Projection, camera.ViewMatrix, ResultMatrix);
	return ViewportPositionMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

glm::vec4 GameObject::LocalPosition() {
	return ResultMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}