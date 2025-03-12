#pragma once
#include "QuadPent_Header.h"

namespace QP {
	class QuadPent_SoundTool {
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
		void Load(QP::Sound& Sound, std::string FileName, FMOD_MODE Option = FMOD_DEFAULT);
		void Release(QP::Sound& Sound);
		void Update();
		void SetFastSoundChannel(QP::FastSoundChannel& ChannelVar, int NumChannel);
		void AddChannelToGroup(QP::SoundChannelGroup& Group, QP::Sound& Sound, QP::SoundChannel& ChannelVar);
		void DeleteChannelFromGroup(QP::SoundChannelGroup& Group, QP::SoundChannel& ChannelVar);
		void ClearGroup(QP::SoundChannelGroup& Group);
		void PlayGroup(QP::SoundChannelGroup& Group);
		void StopGroup(QP::SoundChannelGroup& Group);
		void SetGroupVolume(QP::SoundChannelGroup& Group, float Volume);
		void SetGroupPlaySpeed(QP::SoundChannelGroup& Group, float Speed);
		void PauseGroup(QP::SoundChannelGroup& Group, bool Flag);
		void Play(QP::Sound& Sound, QP::SoundChannel& ChannelVar, float Time = 0.0);
		void Play(QP::Sound& Sound, QP::FastSoundChannel& ChannelVar, float Time=0.0);
		void PlayOnce(QP::Sound& Sound, QP::SoundChannel& ChannelVar, bool& BoolValue, float Time = 0.0);
		void Pause(QP::SoundChannel& ChannelVar, bool Flag);
		void Stop(QP::SoundChannel& ChannelVar);
		void FadeOut(QP::SoundChannel& ChannelVar, float Speed, float FrameTime);
		void FadeIn(QP::SoundChannel& ChannelVar, float Speed, float FrameTime, float DestVolume = 1.0);
		unsigned int GetLength(QP::Sound& Sound);
		unsigned int GetPlayTime(QP::SoundChannel& ChannelVar);
		void SetPlaySpeed(QP::SoundChannel& ChannelVar, float Speed);
		void SetPlaySpeed(QP::FastSoundChannel& ChannelVar, float Speed);
		void SetVolume(QP::SoundChannel& ChannelVar, float Volume);
		void SetVolume(QP::FastSoundChannel& ChannelVar, float Volume);
		void SetPlayTime(QP::SoundChannel& ChannelVar, float Time);
		void ResetPlaySpeed(QP::SoundChannel& ChannelVar);
		void ResetPlaySpeed(QP::FastSoundChannel& ChannelVar);
		void SetFreqCutOff(QP::SoundChannel& ChannelVar, float Frequency);
		void SetBeatDetect(QP::SoundChannel& ChannelVar);
		float DetectBeat(float Threshold, int SamplingRate);
		void DetectBeat(float& DestValue, float Threshold, int SamplingRate);
		bool IsBeat(float Threshold, int SamplingRate);
		void DisableBeatDetect(QP::SoundChannel& ChannelVar);
		void DisableFreqCutOff(QP::SoundChannel& ChannelVar);
		void SetDistance(QP::SoundChannel& ChannelVar, float MinDist, float MaxDist);
		void SetListnerPosition(float X, float Y);
		void SetListnerPosition(QP::Vector2& Position);
		void SetSoundPosition(QP::SoundChannel& ChannelVar, float X, float Y, float Diff);
		void SetSoundPosition(QP::SoundChannel& ChannelVar, QP::Vector2& Position, float Diff);
		void StopAllSounds();
	};

	extern QP::QuadPent_SoundTool SoundTool;
}
