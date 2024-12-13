#pragma once
#include "CollisionUtil.h"
#include "PhysicsUtil.h"
#include "RandomUtil.h"
#include "imageUtil.h"
#include "TextUtil.h"
#include "TimerUtil.h"
#include "TransformUtil.h"
#include "AnimationUtil.h"
#include "ClippingUtil.h"
#include "MathUtil.h"
#include "UiUtil.h"
#include "ExUtil.h"
#include "LineBrush.h"
#include "RectBrush.h"
#include "SystemResource.h"
#include "GameResource.h"
#include "ThreadUtil.h"
#include "ComputeUtil.h"
#include "StringUtil.h"
#include "GlobalValue.h"

#ifdef USE_SOUND_SYSTEM
#include "SoundUtil.h"
#endif

#ifdef USE_FILE_SYSTEM
#include "ScriptUtil.h"
#include "FileUtil.h"
#endif

class GameObject {
public:
	glm::mat4   TranslateMatrix{ 1.0f }, RotateMatrix{ 1.0f }, ScaleMatrix{ 1.0f }, ResultMatrix{ 1.0f };
	glm::mat4   ImageAspectMatrix{ 1.0f }, FlipMatrix{ 1.0f };

	glm::mat4   UnitTranslateMatrix{ 1.0f }, UnitRotateMatrix{ 1.0f }, UnitScaleMatrix{ 1.0f };
	glm::mat4   UnitFlipMatrix{ 1.0f };

	glm::mat4   ViewportPositionMatrix{ 1.0f };

	glm::vec3   ObjectColor{ glm::vec3(0.0, 0.0, 0.0) };

	GLfloat     Opacity{ 1.0f };
	GLfloat     UnitOpacity{ 1.0f };
	GLfloat     BlurValue{};
	GLfloat     UnitBlurValue{};

	std::string ObjectTag{};
	int         ObjectLayer{};

	bool        FloatingOpt{};
	bool        StaticOpt{};
	
	bool        DeleteCommand{};
	bool        SwapCommand{};

	// Initialize matrix. Select a rendering type.
	void InitRenderState(int RenderType=RENDER_TYPE_DEFAULT);

	// Overlays color over the image.
	void SetColor(GLfloat R, GLfloat G, GLfloat B);

	// Overlays color over the image.
	void SetColor(glm::vec3& Color);

	// Overlays color over the image.
	void SetColorRGB(int R, int G, int B);

	// Updates the object's position relative to the viewport. Choose whether to apply aspect ratio.
	void UpdateViewportPosition(GLfloat& DestX, GLfloat& DestY, bool ApplyAspect = true);

	// Updates the object's position relative to the viewport. Choose whether to apply aspect ratio.
	void UpdateViewportPosition(glm::vec2& DestValue, bool ApplyAspect);

	// Updates the object's position relative to the local coordinate system.
	void UpdateLocalPosition(GLfloat& DestX, GLfloat& DestY);

	// Updates the object's position relative to the local coordinate system.
	void UpdateLocalPosition(glm::vec2& DestPosition);

	void UnitFlip(int FlipOpt);
	void SetUnitOpacity(GLfloat Value);
	void SetUnitBlur(int Value);
	void ResetUnitTransform();

	void Flip(int FlipOpt);
	void Blur(int Strength);

	//  Render the image.
	void Render(Image& Image, GLfloat OpacityValue = 1.0, bool ApplyUnitTransform=false, bool DisableAdjustAspect=false);

	// Render the image in one step.
	void DrawImage(int RenderType, Image& ImageSturct, 
		GLfloat X, GLfloat Y, GLfloat Width, GLfloat Height, GLfloat Rotation=0.0, GLfloat OpacityValue=1.0, 
		int FlipOpt=FLIP_TYPE_NONE, bool ApplyUnitTransform=false, bool DisableAdjustAspect=false);

	// Render the image in one step.
	void DrawImage(int RenderType, Image& Image, 
		glm::vec2& Position, GLfloat Width, GLfloat Height, GLfloat Rotation=0.0, GLfloat OpacityValue =1.0,
		int FlipOpt=FLIP_TYPE_NONE, bool ApplyUnitTransform=false, bool DisableAdjustAspect=false);

#ifdef USE_SOUND_SYSTEM
	// Play sound.You can specify the playback start point.
	void PlaySound(Sound Sound, SoundChannel& ChannelVar, unsigned int StartTime=0);
	
	//Play the sound only once. You can specify the playback start point.
	void PlaySoundOnce(Sound Sound, SoundChannel& ChannelVar, bool& BoolValue, unsigned int StartTime=0);

	// Pause the sound. It is played when false is entered in Flag.
	void PauseSound(SoundChannel& ChannelVar, bool Flag);

	// Stop the sound.
	void StopSound(SoundChannel& ChannelVar);

	// Adjust the sound playback speed.
	void SetPlaySpeed(SoundChannel& ChannelVar, float PlaySpeed);

	// Resets sound playback speed to default.
	void ResetPlaySpeed(SoundChannel& ChannelVar);

	// Activates the frequency cutoff effect.
	void EnableFreqCutoff(SoundChannel& ChannelVar, float Frequency);

	// Activates beat detection.
	void EnableBeatDetect(SoundChannel& ChannelVar);

	// Detects the beats of sound in real time. 
	// Applies the detected result to a specific value. You can adjust detection sensitivity with ThreasHold and SamplingRate.
	void DetectBeat(GLfloat& Value, float ThresHold, float SamplingRate);

	// Recognizes the beats of sound in real time.
	// Returns the detected result value.You can adjust detection sensitivity with ThresHold and SamplingRate.
	GLfloat DetectBeat(float ThresHold, float SamplingRate);

	// Detects the beats of sound in real time. 
	// Returns whether the bit is detected. You can adjust detection sensitivity with ThresHold and SamplingRate.
	bool IsBeat(float ThresHold, float SamplingRate);

	// Disables the frequency cutoff effect.
	void DisableFreqCutoff(SoundChannel& ChannelVar);

	// Disables beat detection.
	void DisableBeatDetect(SoundChannel& ChannelVar);

	// Specifies how much the sound volume changes over distance.
	void SetSoundDistance(SoundChannel& ChannelVar, float MinDist, float MaxDist);

	// Specifies the local coordinates at which you hear the sound.
	void SetListnerPosition(float X, float Y);

	// Specifies the local coordinates at which you hear the sound.
	void SetListnerPosition(glm::vec2& Position);

	// Specifies the local coordinates where the sound occurs.
	void SetSoundPosition(SoundChannel& ChannelVar, float X, float Y, float Diff);

	// Specifies the local coordinates where the sound occurs.
	void SetSoundPosition(SoundChannel& ChannelVar, glm::vec2 Position, float Diff);
#endif
	

	// class destructor
	virtual ~GameObject() {}

	// Updates a value inside an object class.
	virtual void UpdateFunc(float FT) {}

	// Renders image resources used by object classes.
	virtual void RenderFunc() {}

	// Sends keyboard events to a specific object class.
	virtual void InputKey(int State, unsigned char NormalKey, int SpecialKey) {}

	// Sends mouse events to a specific object class.
	virtual void InputMouse(int State) {}

	// Sends mouse scroll events to a specific object class.
	virtual void InputScroll(int State) {}

	// Initializes the control state for a specific object class.
	virtual void ResetControlState() {}

	// Obtains an AABB object from a specific object class.
	virtual AABB GetAABB() { return {}; }

	// Obtains an OOBB object from a specific object class.
	virtual OOBB GetOOBB() { return{}; }

	// Obtains an BoundingCircle object from a specific object class.
	virtual BoundingCircle GetBoundingCircle() { return {}; }

private:
	void PrepareRender();
	glm::vec4 ViewportPosition();
	glm::vec4 LocalPosition();
};