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
	case FLIP_TYPE_X:
		FlipMatrix = rotate(FlipMatrix, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		break;

	case FLIP_TYPE_Y:
		FlipMatrix = rotate(FlipMatrix, glm::radians(180.0f), glm::vec3(1.0, 0.0, 0.0));
		break;

	case FLIP_TYPE_XY:
		FlipMatrix = rotate(FlipMatrix, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		FlipMatrix = rotate(FlipMatrix, glm::radians(180.0f), glm::vec3(1.0, 0.0, 0.0));
		break;
	}
}

void GameObject::Blur(int Strength) {
	BlurValue = (GLfloat)Strength;
}

void GameObject::Render(Image& Image, GLfloat Transparency, bool DisableAdjustAspect) {
	TransparencyValue = Transparency;

	if (!DisableAdjustAspect) {
		if (Image.Width > Image.Height)
			ImageAspectMatrix = glm::scale(ImageAspectMatrix, glm::vec3(1.0, (GLfloat)Image.Height / (GLfloat)Image.Width, 1.0));
		else if (Image.Width < Image.Height) 
			ImageAspectMatrix = glm::scale(ImageAspectMatrix, glm::vec3((GLfloat)Image.Width / (GLfloat)Image.Height, 1.0, 1.0));
	}

	Compt::ComputeMatrix(ResultMatrix, TranslateMatrix, RotateMatrix, ScaleMatrix, ImageAspectMatrix, FlipMatrix);

	PrepareRender();
	imageUtil.Render(Image);
}

void GameObject::RenderImage(int RenderType, Image& Image, GLfloat X, GLfloat Y, GLfloat Width, GLfloat Height, GLfloat Rotation, GLfloat Transparency, int FlipOpt, bool DisableAdjustAspect) {
	InitRenderState(RenderType);
	Transform::Move(TranslateMatrix, X, Y);
	Transform::Rotate(RotateMatrix, Rotation);
	Transform::Scale(ScaleMatrix, Width, Height);
	Flip(FlipOpt);
	TransparencyValue = Transparency;
	Render(Image, Transparency, DisableAdjustAspect);
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
	return camera.Projection * camera.ViewMatrix * ResultMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

glm::vec4 GameObject::LocalPosition() {
	return ResultMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}