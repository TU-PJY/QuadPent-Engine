#include "Base.h"
#include "ImageUtil.h"
#include "RenderModeUtil.h"

void BASE::Move(GLfloat X, GLfloat Y) {
	TranslateMatrix = translate(TranslateMatrix, glm::vec3(X, Y, 0.0));
}

void BASE::Rotate(GLfloat RotationValue) {
	RotateMatrix = rotate(RotateMatrix, glm::radians(RotationValue), glm::vec3(0.0, 0.0, 1.0));
}

void BASE::RotateHorziontal(GLfloat RotationValue) {
	RotateMatrix = rotate(RotateMatrix, glm::radians(RotationValue), glm::vec3(1.0, 0.0, 0.0));
}

void BASE::RotateVertical(GLfloat RotationValue) {
	RotateMatrix = rotate(RotateMatrix, glm::radians(RotationValue), glm::vec3(0.0, 1.0, 0.0));
}

void BASE::Scale(GLfloat X, GLfloat Y) {
	ScaleMatrix = scale(ScaleMatrix, glm::vec3(X, Y, 0.0));
}

void BASE::RotateSpot(GLfloat RotationValue) {
	TranslateMatrix = rotate(TranslateMatrix, glm::radians(RotationValue), glm::vec3(0.0, 0.0, 1.0));
}

void BASE::RotateHorizontalSpot(GLfloat RotationValue) {
	TranslateMatrix = rotate(TranslateMatrix, glm::radians(RotationValue), glm::vec3(1.0, 0.0, 0.0));
}

void BASE::RotateVerticalSpot(GLfloat RotationValue) {
	TranslateMatrix = rotate(TranslateMatrix, glm::radians(RotationValue), glm::vec3(0.0, 1.0, 0.0));
}

void BASE::ScaleSpot(GLfloat X, GLfloat Y) {
	TranslateMatrix = scale(TranslateMatrix, glm::vec3(X, Y, 0.0));
}

void BASE::RotateAxis(GLfloat RotationValue, GLfloat AxisX, GLfloat AxisY) {
	TranslateMatrix = translate(TranslateMatrix, glm::vec3(AxisX, AxisY, 0.0));
	TranslateMatrix = rotate(TranslateMatrix, glm::radians(RotationValue), glm::vec3(0.0, 0.0, 1.0));
	TranslateMatrix = translate(TranslateMatrix, glm::vec3(-AxisX, -AxisY, 0.0));
}

void BASE::MoveStraight(GLfloat& Position, int MoveDirection, GLfloat Speed, float FT) {
	Position += Speed * MoveDirection * FT;
}

void BASE::MoveStraight(GLfloat& Position, GLfloat Speed, float FT) {
	Position += Speed * FT;
}

void BASE::MoveForward(GLfloat& X, GLfloat& Y, GLfloat Speed, int MoveDirection, GLfloat RotationValue, float FT, bool Plus90Option) {
	if (Plus90Option) {
		X += Speed * cos(glm::radians(RotationValue + 90)) * MoveDirection * FT;
		Y += Speed * sin(glm::radians(RotationValue + 90)) * MoveDirection * FT;
	}
	else {
		X += Speed * cos(glm::radians(RotationValue)) * MoveDirection * FT;
		Y += Speed * sin(glm::radians(RotationValue)) * MoveDirection * FT;
	}
}

void BASE::MoveForward(GLfloat& X, GLfloat& Y, GLfloat Speed, GLfloat RotationValue, float FT, bool Plus90Option) {
	if (Plus90Option) {
		X += Speed * cos(glm::radians(RotationValue + 90)) * FT;
		Y += Speed * sin(glm::radians(RotationValue + 90)) * FT;
	}
	else {
		X += Speed * cos(glm::radians(RotationValue)) * FT;
		Y += Speed * sin(glm::radians(RotationValue)) * FT;
	}
}

void BASE::LookAt(GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY, GLfloat& RotationVar, GLfloat RotationSpeed, float FT) {
	GLfloat targetAngle{}, shortestAngle{};
	targetAngle = atan2(ToY - FromY, ToX - FromX) * (180 / 3.14) - 90;
	targetAngle = NormalizeDegree(targetAngle);

	if (RotationSpeed > 0)
		shortestAngle = std::lerp(shortestAngle, CalculateShortestRotation(RotationVar, targetAngle), FT * RotationSpeed);
	else
		shortestAngle = CalculateShortestRotation(RotationVar, targetAngle);

	RotationVar = NormalizeDegree(RotationVar + shortestAngle);
}

void BASE::LookAt(GLfloat Rotation, GLfloat& RotationVar, GLfloat RotationSpeed, float FT) {
	GLfloat targetAngle{}, shortestAngle{};
	targetAngle = NormalizeDegree(Rotation);

	if (RotationSpeed > 0)
		shortestAngle = std::lerp(shortestAngle, CalculateShortestRotation(RotationVar, targetAngle), FT * RotationSpeed);
	else
		shortestAngle = CalculateShortestRotation(RotationVar, targetAngle);

	RotationVar = NormalizeDegree(RotationVar + shortestAngle);
}

GLfloat BASE::ASP(GLfloat Value) {
	return Value * ASPECT;
}

void BASE::InitTransform() {
	TranslateMatrix = glm::mat4(1.0f);
	RotateMatrix = glm::mat4(1.0f);
	ScaleMatrix = glm::mat4(1.0f);
	TransparencyValue = 1.0f;
}

void BASE::SetColor(GLfloat R, GLfloat G, GLfloat B) {
	ObjectColor.r = R;
	ObjectColor.g = G;
	ObjectColor.b = B;
}

void BASE::SetImage(unsigned int& Image, std::string ImageName) {
	imageUtil.SetImage(Image, ImageName);
}

void BASE::RenderImage(unsigned int Image, GLfloat Transparency, Flip FlipOption, GLfloat ImageWidth, GLfloat ImageHeight) {
	if (ImageWidth != 0 && ImageHeight != 0)
		TranslateMatrix = scale(TranslateMatrix, glm::vec3(ImageWidth / ImageHeight, 1.0, 0.0));

	if (FlipOption != static_cast<Flip>(-1)) {
		switch (FlipOption) {
		case Flip::Horizontal:
			TranslateMatrix = rotate(TranslateMatrix, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
			break;

		case Flip::Vertical:
			TranslateMatrix = rotate(TranslateMatrix, glm::radians(180.0f), glm::vec3(1.0, 0.0, 0.0));
			break;
		}
	}

	TransparencyValue = Transparency;
	ProcessTransform();
	imageUtil.Render(Image);
}

void BASE::SetSound(Sound& Sound, std::string SoundName) {
	Sound = soundUtil.GetSound(SoundName);
}

void BASE::PlaySound(Sound Sound, Channel& Channel, unsigned int MS) {
	soundUtil.PlaySound(Sound, Channel, MS);
}

void BASE::PauseSound(Channel& Channel, bool Flag) {
	soundUtil.PauseSound(Channel, Flag);
}

void BASE::StopSound(Channel& Channel) {
	soundUtil.StopSound(Channel);
}

void BASE::SetPlaySpeed(Channel& Channel, float PlaySpeed) {
	soundUtil.SetPlaySpeed(Channel, PlaySpeed);
}

void BASE::ResetPlaySpeed(Channel& Channel) {
	soundUtil.ResetPlaySpeed(Channel);
}

void BASE::SetFreqCutOff(Channel& Channel, float Frequency) {
	soundUtil.SetFreqCutOff(Channel, Frequency);
}

void BASE::SetBeatDetect(Channel& Channel) {
	soundUtil.SetBeatDetect(Channel);
}

void BASE::DetectBeat(float Threshold, int SamplingRate) {
	soundUtil.DetectBeat(Threshold, SamplingRate);
}

void BASE::UnsetFreqCutOff(Channel& Channel) {
	soundUtil.UnSetFreqCutOff(Channel);
}

void BASE::UnsetBeatDetect(Channel& Channel) {
	soundUtil.UnSetBeatDetect(Channel);
}

void BASE::SetChannelDistance(Channel& Channel, float MinDist, float MaxDist) {
	soundUtil.SetDistance(Channel, MinDist, MaxDist);
}

void BASE::SetListnerPosition(float X, float Y) {
	soundUtil.SetListnerPosition(X, Y);
}

void BASE::SetSoundPosition(Channel& Channel, float X, float Y) {
	soundUtil.SetSoundPosition(Channel, X, Y);
}

void BASE::InputMousePosition(GLfloat X, GLfloat Y) {
	MouseX = X;
	MouseY = Y;
}

////////////////////////// private

void BASE::ProcessTransform() {
	renderMode.SetImageMode();

	TransparencyLocation = glGetUniformLocation(ImageShader, "transparency");
	glUniform1f(TransparencyLocation, TransparencyValue);

	ObjectColorLocation = glGetUniformLocation(TextShader, "objectColor");
	glUniform3f(ObjectColorLocation, ObjectColor.r, ObjectColor.g, ObjectColor.b);

	ModelLocation = glGetUniformLocation(ImageShader, "model");
	glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, value_ptr(ScaleMatrix * RotateMatrix * TranslateMatrix));
}

GLfloat BASE::NormalizeDegree(GLfloat Degree) {
	while (Degree < 0) Degree += 360;
	while (Degree >= 360) Degree -= 360;
	return Degree;
}

GLfloat BASE::CalculateShortestRotation(GLfloat CurrentDegree, GLfloat DegreeDest) {
	float Diff = DegreeDest - CurrentDegree;

	if (Diff > 180)
		Diff -= 360;
	else if (Diff < -180)
		Diff += 360;

	return Diff;
}