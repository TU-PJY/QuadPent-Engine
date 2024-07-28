#pragma once
#include "CollisionUtil.h"
#include "PhysicsUtil.h"
#include "RandomUtil.h"
#include "SoundUtil.h"
#include "TextUtil.h"
#include "TimerUtil.h"
#include "AnimationUtil.h"

using Sound = FMOD::Sound*;
using Channel = FMOD::Channel*;
using Image = unsigned int;

enum class Flip
{Horizontal, Vertical};

enum class ImageRenderMode
{Static, Default};

class BASE {
private:
	unsigned int ModelLocation{}, TransparencyLocation{}, ObjectColorLocation{}, TextAlphaLocation{};

public:
	std::string ObjectTag{};
	int PrevLayer{};
	int DestLayer{};
	bool SwapLayerDesc{};
	bool DeleteDesc{};
	bool FloatingObjectDesc{};
	bool StaticDesc{};

	glm::mat4 TranslateMatrix{ 1.0f }, RotateMatrix{ 1.0f }, ScaleMatrix{ 1.0f };
	glm::vec3 ObjectColor{ glm::vec3(0.0, 0.0, 0.0) };
	GLfloat TransparencyValue{ 1.0f };

	GLfloat MouseX{}, MouseY{};

	void Move(GLfloat MoveX, GLfloat MoveY);
	void Rotate(GLfloat RotationValue);
	void RotateHorziontal(GLfloat RotationValue);
	void RotateVertical(GLfloat RotationValue);
	void Scale(GLfloat ScaleX, GLfloat ScaleY);
	void MoveStraight(GLfloat& Position, int MoveDirection, GLfloat Speed, float FT);
	void MoveStraight(GLfloat& Position, GLfloat Speed, float FT);
	void MoveForward(GLfloat& X, GLfloat& Y, GLfloat Speed, int MoveDirection, GLfloat RotationValue, float FT, bool Plus90Option=false);
	void MoveForward(GLfloat& X, GLfloat& Y, GLfloat Speed, GLfloat RotationValue, float FT, bool Plus90Option=false);
	GLfloat ASP(GLfloat Value);
	void BeginProcess(ImageRenderMode Mode);
	void RotateAxis(GLfloat RotationValue, GLfloat AxisX, GLfloat AxisY);
	void LookAt(GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY, GLfloat& RotationVar, GLfloat RotationSpeed, float FT);
	void LookAt(GLfloat Rotation, GLfloat& RotationVar, GLfloat RotationSpeed, float FT);
	void SetColor(GLfloat R, GLfloat G, GLfloat B);
	glm::vec4 ViewportPosition();

	void SetImage(unsigned int& Image, std::string ImageName);
	void RenderImage(unsigned int Image, GLfloat Transparency = 1.0, GLfloat ImageWidth = 0, GLfloat ImageHeight = 0, Flip FlipOption = static_cast<Flip>(-1));

	void BeginColorClipping();
	void SetColorClipping();
	void EndColorClipping();

	void BeginTransparentClipping();
	void SetTransparentClipping();
	void EndTransparentClipping();

	void SetSound(Sound& Sound, std::string SoundName);
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
	void SetSoundPosition(Channel& Channel, float X, float Y, float Diff);

	void InputMousePosition(GLfloat X, GLfloat Y);

	// object defined functions
	virtual ~BASE() {}
	virtual void Update(float FT) {}
	virtual void Render() {}
	virtual void InputKey(unsigned char KEY, int S_KEY, bool KeyDown, bool SpecialKey = false) {}
	virtual void InputMouse(int button, int state, int x, int y) {}
	virtual void InputScroll(int button, int Wheel, int x, int y) {}
	virtual void ResetControlState() {}
	virtual AABB GetAABB() { return {}; }
	virtual OBB GetOBB() { return{}; }
	virtual Range GetRange() { return {}; }

private:
	void ProcessTransform(); 
	GLfloat NormalizeDegree(GLfloat Degree);
	GLfloat CalculateShortestRotation(GLfloat CurrentDegree, GLfloat DegreeDest);
};

// dummy object for avoiding iterator error
class FWM_DUMMY : public BASE {};