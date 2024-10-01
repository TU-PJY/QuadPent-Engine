#pragma once
#include "CollisionUtil.h"
#include "PhysicsUtil.h"
#include "RandomUtil.h"
#include "SoundUtil.h"
#include "imageUtil.h"
#include "TextUtil.h"
#include "TimerUtil.h"
#include "AnimationUtil.h"
#include "ClippingUtil.h"
#include "TransformUtil.h"
#include "MathUtil.h"
#include "UiUtil.h"
#include "ExUtil.h"
#include "SystemResources.h"
#include "CustomResources.h"

class GameObject {
private:
	unsigned int ModelLocation{}, TransparencyLocation{}, ObjectColorLocation{}, TextAlphaLocation{};

public:
	const char* ObjectTag{};
	int ObjectLayer{};
	bool SwapLayerMarked{};
	bool DeleteObjectMarked{};
	bool FloatingObjectMarked{};
	bool StaticObjectMarked{};

	glm::mat4 TranslateMatrix{ 1.0f }, RotateMatrix{ 1.0f }, ScaleMatrix{ 1.0f }, ResultMatrix{ 1.0f };
	glm::mat4 ImageAspectMatrix{ 1.0f };
	glm::vec3 ObjectColor{ glm::vec3(0.0, 0.0, 0.0) };
	GLfloat TransparencyValue{ 1.0f };

	// init functions
	void InitMatrix(RenderType Type=RenderType::Default);
	void SetColor(GLfloat R, GLfloat G, GLfloat B);
	void SetColorRGB(int R, int G, int B);

	// viewport functions
	void UpdateViewportPosition(GLfloat& ValueX, GLfloat& ValueY, bool ApplyAspect = true);
	void UpdateLocalPosition(GLfloat& ValueX, GLfloat& ValueY, bool ApplyAspect = false);
	GLfloat ASP(GLfloat Value);

	// image functions
	void RenderImage(Image Image, GLfloat Transparency = 1.0, bool DisableAdjustAspect=false);

	// sound functions
	void PlaySound(Sound Sound, Channel& Channel, unsigned int StartTime);
	void PauseSound(Channel& Channel, bool Flag);
	void StopSound(Channel& Channel);
	void SetPlaySpeed(Channel& Channel, float PlaySpeed);
	void ResetPlaySpeed(Channel& Channel);
	void EnableFreqCutoff(Channel& Channel, float Frequency);
	void EnableBeatDetect(Channel& Channel);
	void DetectBeat(GLfloat& Value, float ThresHold, float SamplingRate);
	bool IsBeat(float ThresHold, float SamplingRate);
	void DisableFreqCutoff(Channel& Channel);
	void DisableBeatDetect(Channel& Channel);
	void SetSoundDistance(Channel& Channel, float MinDist, float MaxDist);
	void SetListnerPosition(float X, float Y);
	void SetSoundPosition(Channel& Channel, float X, float Y, float Diff);

	// object defined functions
	virtual ~GameObject() {}
	virtual void Update(float FT) {}
	virtual void Render() {}
	virtual void InputKey(KeyType Type, KeyState State, unsigned char NormalKey, int SpecialKey) {}
	virtual void InputMouse(int Button, int State, int X, int Y) {}
	virtual void InputScroll(int Button, int Wheel, int X, int Y) {}
	virtual void ResetControlState() {}
	virtual AABB GetAABB() { return {}; }
	virtual OBB GetOBB() { return{}; }
	virtual Range GetRange() { return {}; }

private:
	void PrepareRender(); 
	glm::vec4 ViewportPosition();
	glm::vec4 LocalPosition();
};