#pragma once
#include "SDKHeader.h"
#include "Config.h"
#include <string>

class SDK_SoundTool {
private:
	FMOD::System* SoundSystem{};
	FMOD::DSP*    BeatDetector{};
	FMOD::DSP*    LowPass{};

	FMOD_VECTOR   ListenerPosition{};
	FMOD_VECTOR   SoundPosition{};

	FMOD_RESULT   Result{};
	void*         ExtDvData{};

	std::vector<float> FFTdata{};

public:
	void Init();
	void Load(Sound& Sound, std::string FileName, FMOD_MODE Option=FMOD_DEFAULT);
	void Release(Sound& Sound);
	void Update();
	void AddChannelToGroup(SoundChannelGroup& Group, Sound& Sound, SoundChannel& ChannelVar);
	void DeleteChannelFromGroup(SoundChannelGroup& Group, SoundChannel& ChannelVar);
	void ClearGroup(SoundChannelGroup& Group);
	void PlayGroup(SoundChannelGroup& Group);
	void StopGroup(SoundChannelGroup& Group);
	void PauseGroup(SoundChannelGroup& Group, bool Flag);
	void Play(Sound& Sound, SoundChannel& ChannelVar, float Time=0.0);
	void PlayOnce(Sound& Sound, SoundChannel& ChannelVar, bool& BoolValue, float Time=0.0);
	void Pause(SoundChannel& ChannelVar, bool Flag);
	void Stop(SoundChannel& ChannelVar);
	void FadeOut(SoundChannel& ChannelVar, float Speed, float FrameTime);
	void FadeIn(SoundChannel& ChannelVar, float Speed, float FrameTime, float DestVolume=1.0);
	unsigned int GetLength(Sound& Sound);
	unsigned int GetPlayTime(SoundChannel& ChannelVar);
	void SetPlaySpeed(SoundChannel& ChannelVar, float Speed);
	void SetVolume(SoundChannel& ChannelVar, float Volume);
	void SetPlayTime(SoundChannel& ChannelVar, float Time);
	void ResetPlaySpeed(SoundChannel& ChannelVar);
	void SetFreqCutOff(SoundChannel& ChannelVar, float Frequency);
	void SetBeatDetect(SoundChannel& ChannelVar);
	float DetectBeat(float Threshold, int SamplingRate);
	void DetectBeat(float& DestValue, float Threshold, int SamplingRate);
	bool IsBeat(float Threshold, int SamplingRate);
	void DisableBeatDetect(SoundChannel& ChannelVar);
	void DisableFreqCutOff(SoundChannel& ChannelVar);
	void SetDistance(SoundChannel& ChannelVar, float MinDist, float MaxDist);
	void SetListnerPosition(float X, float Y);
	void SetListnerPosition(glm::vec2 Position);
	void SetSoundPosition(SoundChannel& ChannelVar, float X, float Y, float Diff);
	void SetSoundPosition(SoundChannel& ChannelVar, glm::vec2 Position, float Diff);
	void StopAllSounds();
};
namespace SDK {
	extern SDK_SoundTool SoundTool;
}
