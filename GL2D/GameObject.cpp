#include "GameObject.h"
#include "ImageUtil.h"
#include "CameraUtil.h"
#include <cmath>

// init functions
void GameObject::InitMatrix(RenderType Type) {
	TranslateMatrix = glm::mat4(1.0f);
	RotateMatrix = glm::mat4(1.0f);
	ScaleMatrix = glm::mat4(1.0f);
	ImageAspectMatrix = glm::mat4(1.0f);
	TransparencyValue = 1.0f;

	glUseProgram(ImageShader);
	camera.SetCamera(Type);
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

void GameObject::RenderImage(Image Image, GLfloat Transparency, bool DisableAdjustAspect) {
	TransparencyValue = Transparency;

	if (!DisableAdjustAspect) {
		if (Image.Width > Image.Height)
			ImageAspectMatrix = glm::scale(ImageAspectMatrix, glm::vec3(1.0, (GLfloat)Image.Height / (GLfloat)Image.Width, 1.0));

		else if (Image.Width < Image.Height)
			ImageAspectMatrix = glm::scale(ImageAspectMatrix, glm::vec3((GLfloat)Image.Width / (GLfloat)Image.Height, 1.0, 1.0));
	}

	PrepareRender();
	imageUtil.Render(Image);
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

void GameObject::EnableFreqCutoff(Channel& Channel, float Frequency) {
	soundUtil.SetFreqCutOff(Channel, Frequency);
}

void GameObject::EnableBeatDetect(Channel& Channel) {
	soundUtil.SetBeatDetect(Channel);
}

void GameObject::DetectBeat(GLfloat& Value, float ThresHold, float SamplingRate) {
	Value =  soundUtil.DetectBeat(ThresHold, SamplingRate);
}

void GameObject::DisableFreqCutoff(Channel& Channel) {
	soundUtil.UnSetFreqCutOff(Channel);
}

void GameObject::DisableBeatDetect(Channel& Channel) {
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


////////////////////////// private
void GameObject::PrepareRender() {
	camera.PrepareRender(ShaderType::Image);

	TransparencyLocation = glGetUniformLocation(ImageShader, "transparency");
	glUniform1f(TransparencyLocation, TransparencyValue);

	ObjectColorLocation = glGetUniformLocation(TextShader, "objectColor");
	glUniform3f(ObjectColorLocation, ObjectColor.r, ObjectColor.g, ObjectColor.b);

	ModelLocation = glGetUniformLocation(ImageShader, "model");

	if (ImageAspectMatrix != glm::mat4(1.0f))
		ResultMatrix = TranslateMatrix * RotateMatrix * ScaleMatrix * ImageAspectMatrix;
	else
		ResultMatrix = TranslateMatrix * RotateMatrix * ScaleMatrix;

	glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, value_ptr(ResultMatrix));
}

glm::vec4 GameObject::ViewportPosition() {
	return camera.Projection * camera.ViewMatrix * ResultMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

glm::vec4 GameObject::LocalPosition() {
	return ResultMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}