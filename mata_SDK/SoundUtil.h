#pragma once
#include "SystemConfig.h"

#ifdef USE_SOUND_SYSTEM
#include "SDKHeader.h"
#include <string>

class SoundUtil {
private:
	FMOD::System* SoundSystem{};
	FMOD::DSP*    BeatDetector{};
	FMOD::DSP*    LowPass{};

	FMOD_VECTOR   ListenerPosition{};
	FMOD_VECTOR   SoundPosition{};

	FMOD_RESULT   Result{};
	void*         ExtDvData{};

public:
	void Init();
	void Load(FMOD::Sound*& Sound, std::string FileName, FMOD_MODE Option=FMOD_DEFAULT);
	void Release(FMOD::Sound*& Sound);
	void Update();
	void PlaySound(FMOD::Sound* Sound, FMOD::Channel*& ChannelVar, unsigned int Ms=0);
	void PlaySoundOnce(FMOD::Sound* Sound, FMOD::Channel*& ChannelVar, bool& FlagValue, unsigned int Ms=0);
	void PauseSound(FMOD::Channel*& ChannelVar, bool Flag);
	void StopSound(FMOD::Channel*& ChannelVar);
	unsigned int GetLength(FMOD::Sound* Sound);
	unsigned int GetPlayTime(FMOD::Channel* ChannelVar);
	void SetPlaySpeed(FMOD::Channel*& ChannelVar, float Speed);
	void ResetPlaySpeed(FMOD::Channel*& ChannelVar);
	void SetFreqCutOff(FMOD::Channel*& ChannelVar, float Frequency);
	void SetBeatDetect(FMOD::Channel*& ChannelVar);
	float DetectBeat(float Threshold, float SamplingRate);
	bool IsBeat(float Threshold, float SamplingRate);
	void UnSetBeatDetect(FMOD::Channel*& ChannelVar);
	void UnSetFreqCutOff(FMOD::Channel*& ChannelVar);
	void SetDistance(FMOD::Channel*& ChannelVar, float MinDist, float MaxDist);
	void SetListnerPosition(float X, float Y);
	void SetListnerPosition(glm::vec2 Position);
	void SetSoundPosition(FMOD::Channel*& ChannelVar, float X, float Y, float Diff);
	void SetSoundPosition(FMOD::Channel*& ChannelVar, glm::vec2 Position, float Diff);
	void StopAllSounds();
};
extern SoundUtil soundUtil;
#endif
