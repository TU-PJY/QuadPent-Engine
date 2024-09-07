#include "GameObject.h"
#include "ImageUtil.h"
#include "RenderModeUtil.h"
#include "CameraUtil.h"
#include "InstancingResourceList.h"
#include <cmath>

// init functions
void GameObject::BeginProcess(RenderType Type) {
	TranslateMatrix = glm::mat4(1.0f);
	RotateMatrix = glm::mat4(1.0f);
	ScaleMatrix = glm::mat4(1.0f);
	TransparencyValue = 1.0f;

	if (Type == RenderType::Static)
		renderMode.SetStaticImageMode();
	else
		renderMode.SetImageMode();
}

void GameObject::SetColor(GLfloat R, GLfloat G, GLfloat B) {
	ObjectColor.r = R;
	ObjectColor.g = G;
	ObjectColor.b = B;
}

void GameObject::SetColorRGB(int R, int G, int B) {
	ObjectColor.r = (1.0f / 255.0f) * (GLfloat)R;
	ObjectColor.g = (1.0f / 255.0f) * (GLfloat)G;
	ObjectColor.b = (1.0f / 255.0f) * (GLfloat)B;
}

void GameObject::MoveStraight(GLfloat& Position, int MoveDirection, GLfloat Speed, float FT) {
	Position += Speed * MoveDirection * FT;
}

void GameObject::MoveStraight(GLfloat& Position, GLfloat Speed, float FT) {
	Position += Speed * FT;
}

void GameObject::MoveForward(GLfloat& X, GLfloat& Y, GLfloat Speed, int MoveDirection, GLfloat RotationValue, float FT, bool Plus90Option) {
	if (Plus90Option) {
		X += Speed * cos(glm::radians(RotationValue + 90)) * MoveDirection * FT;
		Y += Speed * sin(glm::radians(RotationValue + 90)) * MoveDirection * FT;
	}
	else {
		X += Speed * cos(glm::radians(RotationValue)) * MoveDirection * FT;
		Y += Speed * sin(glm::radians(RotationValue)) * MoveDirection * FT;
	}
}

void GameObject::MoveForward(GLfloat& X, GLfloat& Y, GLfloat Speed, GLfloat RotationValue, float FT, bool Plus90Option) {
	if (Plus90Option) {
		X += Speed * cos(glm::radians(RotationValue + 90)) * FT;
		Y += Speed * sin(glm::radians(RotationValue + 90)) * FT;
	}
	else {
		X += Speed * cos(glm::radians(RotationValue)) * FT;
		Y += Speed * sin(glm::radians(RotationValue)) * FT;
	}
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

void GameObject::UpdateLocalPosition(GLfloat& ValueX, GLfloat& ValueY, bool ApplyAspect) {
	if(ApplyAspect)
		ValueX = ASP(LocalPosition().x);
	else
		ValueX = LocalPosition().x;
	ValueY = LocalPosition().y;
}

void GameObject::SetImage(Image& ImageValue, std::string ImageName) {
	imageUtil.SetImage(ImageValue, ImageName);
}

void GameObject::RenderImage(Image Image, GLfloat Transparency) {
	TransparencyValue = Transparency;
	ProcessTransform();
	imageUtil.Render(Image);
}

void GameObject::SetSound(Sound& Sound, std::string SoundName) {
	soundUtil.SetSound(Sound, SoundName);
}

void GameObject::PlaySound(Sound Sound, Channel& Channel, unsigned int StartTime) {
	soundUtil.PlaySound(Sound, Channel, StartTime);
}

void GameObject::PauseSound(Channel& Channel, bool Flag) {
	soundUtil.PauseSound(Channel, Flag);
}

void GameObject::StopSound(Channel& Channel) {
	soundUtil.StopSound(Channel);
}

void GameObject::SetPlaySpeed(Channel& Channel, float PlaySpeed) {
	soundUtil.SetPlaySpeed(Channel, PlaySpeed);
}

void GameObject::ResetPlaySpeed(Channel& Channel) {
	soundUtil.ResetPlaySpeed(Channel);
}

void GameObject::SetFreqCutoff(Channel& Channel, float Frequency) {
	soundUtil.SetFreqCutOff(Channel, Frequency);
}

void GameObject::SetBeatDetect(Channel& Channel) {
	soundUtil.SetBeatDetect(Channel);
}

void GameObject::DetectBeat(GLfloat& Value, float ThresHold, float SamplingRate) {
	Value =  soundUtil.DetectBeat(ThresHold, SamplingRate);
}

void GameObject::RemoveFreqCutoff(Channel& Channel) {
	soundUtil.UnSetFreqCutOff(Channel);
}

void GameObject::RemoveBeatDetect(Channel& Channel) {
	soundUtil.UnSetBeatDetect(Channel);
}

void GameObject::SetSoundDistance(Channel& Channel, float MinDist, float MaxDist) {
	soundUtil.SetDistance(Channel, MinDist, MaxDist);
}

void GameObject::SetListnerPosition(float X, float Y) {
	soundUtil.SetListnerPosition(X, Y);
}

void GameObject::SetSoundPosition(Channel& Channel, float X, float Y, float Diff) {
	soundUtil.SetSoundPosition(Channel, X, Y, Diff);
}

int GameObject::GetSoundCount() {
	return soundUtil.GetSoundNum();
}

int GameObject::GetSoundCountIf(std::string ContainedName) {
	return soundUtil.GetSoundNumIf(ContainedName);
}


////////////////////////// private
void GameObject::ProcessTransform() {
	camera.ProcessTransform(false);

	TransparencyLocation = glGetUniformLocation(ImageShader, "transparency");
	glUniform1f(TransparencyLocation, TransparencyValue);

	ObjectColorLocation = glGetUniformLocation(TextShader, "objectColor");
	glUniform3f(ObjectColorLocation, ObjectColor.r, ObjectColor.g, ObjectColor.b);

	ModelLocation = glGetUniformLocation(ImageShader, "model");
	ResultMatrix = TranslateMatrix * RotateMatrix * ScaleMatrix;
	glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, value_ptr(ResultMatrix));
}

glm::vec4 GameObject::ViewportPosition() {
	glm::vec4 Position = camera.Projection * camera.ViewMatrix * ResultMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	return Position;
}

glm::vec4 GameObject::LocalPosition() {
	glm::vec4 LocalPosition = ResultMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	return LocalPosition;
}