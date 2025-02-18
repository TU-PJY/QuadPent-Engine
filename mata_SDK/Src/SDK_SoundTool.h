#pragma once
#include "SDK_Header.h"
#include "SDK_Config.h"
#include <string>

namespace MSDK {
	class SDK_SoundTool {
	private:
		FMOD::System* SoundSystem{};
		FMOD::DSP* BeatDetector{};
		FMOD::DSP* LowPass{};

		FMOD_VECTOR   ListenerPosition{};
		FMOD_VECTOR   SoundPosition{};

		FMOD_RESULT   Result{};
		void* ExtDvData{};

		std::vector<float> FFTdata{};

	public:
		void Init();
		void Load(MSDK::Sound& Sound, std::string FileName, FMOD_MODE Option = FMOD_DEFAULT);
		void Release(MSDK::Sound& Sound);
		void Update();
		void SetFastSoundChannel(MSDK::FastSoundChannel& ChannelVar, int NumChannel);
		void AddChannelToGroup(MSDK::SoundChannelGroup& Group, MSDK::Sound& Sound, MSDK::SoundChannel& ChannelVar);
		void DeleteChannelFromGroup(MSDK::SoundChannelGroup& Group, MSDK::SoundChannel& ChannelVar);
		void ClearGroup(MSDK::SoundChannelGroup& Group);
		void PlayGroup(MSDK::SoundChannelGroup& Group);
		void StopGroup(MSDK::SoundChannelGroup& Group);
		void SetGroupVolume(MSDK::SoundChannelGroup& Group, float Volume);
		void SetGroupPlaySpeed(MSDK::SoundChannelGroup& Group, float Speed);
		void PauseGroup(MSDK::SoundChannelGroup& Group, bool Flag);
		void Play(MSDK::Sound& Sound, MSDK::SoundChannel& ChannelVar, float Time = 0.0);
		void Play(MSDK::Sound& Sound, MSDK::FastSoundChannel& ChannelVar, float Time);
		void PlayOnce(MSDK::Sound& Sound, MSDK::SoundChannel& ChannelVar, bool& BoolValue, float Time = 0.0);
		void Pause(MSDK::SoundChannel& ChannelVar, bool Flag);
		void Stop(MSDK::SoundChannel& ChannelVar);
		void FadeOut(MSDK::SoundChannel& ChannelVar, float Speed, float FrameTime);
		void FadeIn(MSDK::SoundChannel& ChannelVar, float Speed, float FrameTime, float DestVolume = 1.0);
		unsigned int GetLength(MSDK::Sound& Sound);
		unsigned int GetPlayTime(MSDK::SoundChannel& ChannelVar);
		void SetPlaySpeed(MSDK::SoundChannel& ChannelVar, float Speed);
		void SetPlaySpeed(MSDK::FastSoundChannel& ChannelVar, float Speed);
		void SetVolume(MSDK::SoundChannel& ChannelVar, float Volume);
		void SetVolume(MSDK::FastSoundChannel& ChannelVar, float Volume);
		void SetPlayTime(MSDK::SoundChannel& ChannelVar, float Time);
		void ResetPlaySpeed(MSDK::SoundChannel& ChannelVar);
		void ResetPlaySpeed(MSDK::FastSoundChannel& ChannelVar);
		void SetFreqCutOff(MSDK::SoundChannel& ChannelVar, float Frequency);
		void SetBeatDetect(MSDK::SoundChannel& ChannelVar);
		float DetectBeat(float Threshold, int SamplingRate);
		void DetectBeat(float& DestValue, float Threshold, int SamplingRate);
		bool IsBeat(float Threshold, int SamplingRate);
		void DisableBeatDetect(MSDK::SoundChannel& ChannelVar);
		void DisableFreqCutOff(MSDK::SoundChannel& ChannelVar);
		void SetDistance(MSDK::SoundChannel& ChannelVar, float MinDist, float MaxDist);
		void SetListnerPosition(float X, float Y);
		void SetListnerPosition(glm::vec2 Position);
		void SetSoundPosition(MSDK::SoundChannel& ChannelVar, float X, float Y, float Diff);
		void SetSoundPosition(MSDK::SoundChannel& ChannelVar, glm::vec2 Position, float Diff);
		void StopAllSounds();
	};

	extern MSDK::SDK_SoundTool SoundTool;
}
