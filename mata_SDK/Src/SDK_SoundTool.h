#pragma once
#include "SDK_Header.h"
#include "SDK_Config.h"
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
	void Load(SDK::Sound& Sound, std::string FileName, FMOD_MODE Option=FMOD_DEFAULT);
	void Release(SDK::Sound& Sound);
	void Update();
	void AddChannelToGroup(SDK::SoundChannelGroup& Group, SDK::Sound& Sound, SDK::SoundChannel& ChannelVar);
	void DeleteChannelFromGroup(SDK::SoundChannelGroup& Group, SDK::SoundChannel& ChannelVar);
	void ClearGroup(SDK::SoundChannelGroup& Group);
	void PlayGroup(SDK::SoundChannelGroup& Group);
	void StopGroup(SDK::SoundChannelGroup& Group);
	void PauseGroup(SDK::SoundChannelGroup& Group, bool Flag);
	void Play(SDK::Sound& Sound, SDK::SoundChannel& ChannelVar, float Time=0.0);
	void PlayOnce(SDK::Sound& Sound, SDK::SoundChannel& ChannelVar, bool& BoolValue, float Time=0.0);
	void Pause(SDK::SoundChannel& ChannelVar, bool Flag);
	void Stop(SDK::SoundChannel& ChannelVar);
	void FadeOut(SDK::SoundChannel& ChannelVar, float Speed, float FrameTime);
	void FadeIn(SDK::SoundChannel& ChannelVar, float Speed, float FrameTime, float DestVolume=1.0);
	unsigned int GetLength(SDK::Sound& Sound);
	unsigned int GetPlayTime(SDK::SoundChannel& ChannelVar);
	void SetPlaySpeed(SDK::SoundChannel& ChannelVar, float Speed);
	void SetVolume(SDK::SoundChannel& ChannelVar, float Volume);
	void SetPlayTime(SDK::SoundChannel& ChannelVar, float Time);
	void ResetPlaySpeed(SDK::SoundChannel& ChannelVar);
	void SetFreqCutOff(SDK::SoundChannel& ChannelVar, float Frequency);
	void SetBeatDetect(SDK::SoundChannel& ChannelVar);
	float DetectBeat(float Threshold, int SamplingRate);
	void DetectBeat(float& DestValue, float Threshold, int SamplingRate);
	bool IsBeat(float Threshold, int SamplingRate);
	void DisableBeatDetect(SDK::SoundChannel& ChannelVar);
	void DisableFreqCutOff(SDK::SoundChannel& ChannelVar);
	void SetDistance(SDK::SoundChannel& ChannelVar, float MinDist, float MaxDist);
	void SetListnerPosition(float X, float Y);
	void SetListnerPosition(glm::vec2 Position);
	void SetSoundPosition(SDK::SoundChannel& ChannelVar, float X, float Y, float Diff);
	void SetSoundPosition(SDK::SoundChannel& ChannelVar, glm::vec2 Position, float Diff);
	void StopAllSounds();
};
namespace SDK {
	extern SDK_SoundTool SoundTool;
}
