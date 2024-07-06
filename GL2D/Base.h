#pragma once
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <string>
#include "SoundUtil.h"

using Sound = FMOD::Sound*;
using Channel = FMOD::Channel*;

enum class Flip
{Horizontal, Vertical};

class BASE {
private:
	unsigned int ModelLocation{}, TransparencyLocation{}, ObjectColorLocation{}, TextAlphaLocation{};

public:
	bool DeleteDesc{};
	bool FloatingObjectDesc{};
	std::string ObjectTag{};

	glm::mat4 TranslateMatrix{ 1.0f }, RotateMatrix{ 1.0f }, ScaleMatrix{ 1.0f };
	glm::vec3 ObjectColor{ glm::vec3(0.0, 0.0, 0.0) };
	GLfloat TransparencyValue{ 1.0f };

	void Move(GLfloat MoveX, GLfloat MoveY);
	void Rotate(GLfloat Radians);
	void RotateHorziontal(GLfloat Radians);
	void RotateVertical(GLfloat Radians);
	void Scale(GLfloat ScaleX, GLfloat ScaleY);
	void RotateSpot(GLfloat Radians);
	void RotateHorizontalSpot(GLfloat Radians);
	void RotateVerticalSpot(GLfloat Radians);
	void ScaleSpot(GLfloat X, GLfloat Y);
	void MoveStraight(GLfloat& Position, int MoveDirection, GLfloat Speed, float FT);
	void MoveForward(GLfloat& X, GLfloat& Y, GLfloat Speed, int MoveDirection, GLfloat Radians, float FT);
	GLfloat ASP(GLfloat Value);
	void RotateAxis(GLfloat Radians, GLfloat AxisX, GLfloat AxisY);
	void SetColor(GLfloat R, GLfloat G, GLfloat B);
	void InitTransform();

	void SetImage(unsigned int& Image, std::string ImageName);
	void RenderImage(unsigned int Image, GLfloat Transparency, Flip FlipOption=static_cast<Flip>(-1), GLfloat ImageWidth = 0, GLfloat ImageHeight = 0);

	void SetSound(Sound& Sound, std::string SoundName);
	void SetChannel(Channel& Channel, std::string ChannelName);
	void PlaySound(Sound Sound, Channel& Channel, unsigned int MS = 0);
	void PauseSound(Channel& Channel, bool Flag);
	void StopSound(Channel& Channel);
	void SetPlaySpeed(Channel& Channel, float PlaySpeed);
	void ResetPlaySpeed(Channel& Channel);
	void SetFreqCutOff(Channel& Channel, float Frequency);
	void SetBeatDetect(Channel& Channel);
	void DetectBeat(float Threshold, int SamplingRate);
	void UnsetFreqCutOff(Channel& Channel);
	void UnsetBeatDetect(Channel& Channel);

	void SetChannelDistance(Channel& Channel, float MinDist, float MaxDist);

	void SetListnerPosition(float X, float Y);

	void SetSoundPosition(Channel& Channel, float X, float Y);

	// essential functions
	virtual void Update(float FT) {}
	virtual void Render() {}
	virtual void InputKey(unsigned char KEY, int S_KEY, bool KeyDown, bool SpecialKey = false) {}
	virtual void InputMouse(int button, int state, int x, int y) {}
	virtual void InputScroll(int button, int Wheel, int x, int y) {}
	virtual void ResetControlState() {}

private:
	void ProcessTransform();
};

// dummy object for avoiding iterator error
class FWM_DUMMY : public BASE {};