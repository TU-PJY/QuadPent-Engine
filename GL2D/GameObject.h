#pragma once
#include "CollisionUtil.h"
#include "PhysicsUtil.h"
#include "RandomUtil.h"
#include "SoundUtil.h"
#include "imageUtil.h"
#include "TextUtil.h"
#include "TimerUtil.h"
#include "AnimationUtil.h"

using Sound = FMOD::Sound*;
using Channel = FMOD::Channel*;
using Image = unsigned int;

enum class FlipDir
{H, V};

class GameObject {
private:
	unsigned int ModelLocation{}, TransparencyLocation{}, ObjectColorLocation{}, TextAlphaLocation{};

public:
	std::string ObjectTag{};
	int PrevLayer{};
	int DestLayer{};
	bool SwapLayerMarked{};
	bool DeleteObjectMarked{};
	bool FloatingObjectMarked{};
	bool StaticObjectMarked{};

	glm::mat4 TranslateMatrix{ 1.0f }, RotateMatrix{ 1.0f }, ScaleMatrix{ 1.0f };
	glm::vec3 ObjectColor{ glm::vec3(0.0, 0.0, 0.0) };
	GLfloat TransparencyValue{ 1.0f };

	// init functions
	void BeginProcess(RenderType Type);
	void SetColor(GLfloat R, GLfloat G, GLfloat B);
	void SetColorRGB(int R, int G, int B);

	// move functions
	void MoveStraight(GLfloat& Position, int MoveDirection, GLfloat Speed, float FT);
	void MoveStraight(GLfloat& Position, GLfloat Speed, float FT);
	void MoveForward(GLfloat& X, GLfloat& Y, GLfloat Speed, int MoveDirection, GLfloat RotationValue, float FT, bool Plus90Option = false);
	void MoveForward(GLfloat& X, GLfloat& Y, GLfloat Speed, GLfloat RotationValue, float FT, bool Plus90Option = false);

	// viewport functions
	void UpdateViewportPosition(GLfloat& ValueX, GLfloat& ValueY, bool ApplyAspect = true);
	GLfloat ASP(GLfloat Value);

	// image functions
	void SetImage(Image& ImageValue, std::string ImageName);
	void RenderImage(Image Image, GLfloat Transparency = 1.0);

	// sound functions
	void SetSound(Sound& Sound, std::string SoundName);
	void PlaySound(Sound Sound, Channel& Channel, unsigned int StartTime);
	void PauseSound(Channel& Channel, bool Flag);
	void StopSound(Channel& Channel);
	void SetPlaySpeed(Channel& Channel, float PlaySpeed);
	void ResetPlaySpeed(Channel& Channel);
	void SetFreqCutoff(Channel& Channel, float Frequency);
	void SetBeatDetect(Channel& Channel);
	void DetectBeat(GLfloat& Value, float ThresHold, float SamplingRate);
	void RemoveFreqCutoff(Channel& Channel);
	void RemoveBeatDetect(Channel& Channel);
	void SetSoundDistance(Channel& Channel, float MinDist, float MaxDist);
	void SetListnerPosition(float X, float Y);
	void SetSoundPosition(Channel& Channel, float X, float Y, float Diff);
	int GetSoundCount();
	int GetSoundCountIf(std::string ContainedName);

	// object defined functions
	virtual ~GameObject() {}
	virtual void Update(float FT) {}
	virtual void Render() {}
	virtual void InputKey(KeyType Key, KeyState State, unsigned char NormalKey, int SpecialKey) {}
	virtual void InputMouse(int Button, int State, int X, int Y) {}
	virtual void InputScroll(int Button, int Wheel, int X, int Y) {}
	virtual void ResetControlState() {}
	virtual AABB GetAABB() { return {}; }
	virtual OBB GetOBB() { return{}; }
	virtual Range GetRange() { return {}; }

private:
	void ProcessTransform(); 
	glm::vec4 ViewportPosition();
};

// dummy object for avoiding iterator error
class __DUMMY__ : public GameObject {};


namespace Transform {
	void Move(glm::mat4& Matrix, GLfloat X, GLfloat Y);
	void Rotate(glm::mat4& Matrix, GLfloat Degree);
	void Scale(glm::mat4& Matrix, GLfloat X, GLfloat Y);
	void RotateV(glm::mat4& Matrix, GLfloat Degree);
	void RotateH(glm::mat4& Matrix, GLfloat Degree);
	void Flip(glm::mat4& Matrix, FlipDir FlipOption);
	void MatchAspect(glm::mat4& Matrix, GLfloat ImageWidth, GLfloat ImageHeight);
}

namespace Math {
	void LookAt(GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY, GLfloat& RotationVar, GLfloat RotationSpeed, float FT);
	void LookAt(GLfloat Rotation, GLfloat& RotationVar, GLfloat RotationSpeed, float FT);
	GLfloat CalcDistance(GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY);
	GLfloat NormalizeDegree(GLfloat Degree);
	GLfloat CalculateShortestRotation(GLfloat CurrentDegree, GLfloat DegreeDest);
}

namespace ColorClipping {
	void First();
	void Second();
	void End();
}

namespace AlphaClipping {
	void First();
	void Second();
	void End();
}