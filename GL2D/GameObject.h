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
#include "LineBrush.h"
#include "RectBrush.h"
#include "SystemResources.h"
#include "CustomResources.h"

class GameObject {
private:
	unsigned int ModelLocation{}, TransparencyLocation{}, ObjectColorLocation{}, TextAlphaLocation{};
	unsigned int RadiusLocation{}, TexelSizeLocation{}, BoolBlurLocation{};

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
	GLfloat BlurValue{};

	// init functions
	void InitMatrix(int RenderType=RENDER_TYPE_DEFAULT);
	void SetColor(GLfloat R, GLfloat G, GLfloat B);
	void SetColorRGB(int R, int G, int B);

	// viewport functions
	void UpdateViewportPosition(GLfloat& ValueX, GLfloat& ValueY, bool ApplyAspect = true);
	void UpdateLocalPosition(GLfloat& ValueX, GLfloat& ValueY, bool ApplyAspect = false);
	GLfloat ASP(GLfloat Value);

	// image functions
	void RenderImage(Image& Image, GLfloat Transparency = 1.0, bool DisableAdjustAspect=false);
	void SetBlur(int Strength);

	// sound functions
	void PlaySound(Sound Sound, SoundChannel& ChannelVar, unsigned int StartTime);
	void PauseSound(SoundChannel& ChannelVar, bool Flag);
	void StopSound(SoundChannel& ChannelVar);
	void SetPlaySpeed(SoundChannel& ChannelVar, float PlaySpeed);
	void ResetPlaySpeed(SoundChannel& ChannelVar);
	void EnableFreqCutoff(SoundChannel& ChannelVar, float Frequency);
	void EnableBeatDetect(SoundChannel& ChannelVar);
	void DetectBeat(GLfloat& Value, float ThresHold, float SamplingRate);
	bool IsBeat(float ThresHold, float SamplingRate);
	void DisableFreqCutoff(SoundChannel& ChannelVar);
	void DisableBeatDetect(SoundChannel& ChannelVar);
	void SetSoundDistance(SoundChannel& ChannelVar, float MinDist, float MaxDist);
	void SetListnerPosition(float X, float Y);
	void SetSoundPosition(SoundChannel& ChannelVar, float X, float Y, float Diff);

	// object defined functions
	virtual ~GameObject() {}
	virtual void Update(float FT) {}
	virtual void Render() {}
	virtual void InputKey(int Type, int State, unsigned char NormalKey, int SpecialKey) {}
	virtual void InputMouse(int Button, int State, int X, int Y) {}
	virtual void InputScroll(int Button, int Wheel, int X, int Y) {}
	virtual void ResetControlState() {}
	virtual AABB GetAABB() { return {}; }
	virtual OOBB GetOBB() { return{}; }
	virtual BoundingSphere GetRange() { return {}; }

private:
	void PrepareRender(); 
	glm::vec4 ViewportPosition();
	glm::vec4 LocalPosition();
};