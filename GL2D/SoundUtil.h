#pragma once
#include "fmod.hpp"
#include "fmod_errors.h"
#include <map>
#include <string>

#define FFT_SIZE 1024 // FFT Size

struct FileNameAndOption {
	std::string Name;
	const char* FileName;
	FMOD_MODE Option;
};

class SoundUtil {
private:
	std::map<std::string, FMOD::Sound*> LoadedSoundList;
	std::map<std::string, FMOD::Channel*> LoadedChannelList;
	FMOD::System* SoundSystem{};
	FMOD::DSP* BeatDetector{};
	FMOD::DSP* LowPass{};

	FMOD_VECTOR listenerPos{};
	FMOD_VECTOR soundPos{};

	FMOD_RESULT Result{};
	void* ExtDvData{};

public:
	void Init();
	FMOD::Channel* GetChannel(std::string ChannelName);
	FMOD::Sound* GetSound(std::string SoundName);
	void LoadSoundFromList();
	void LoadChannelFromList();
	void Update();
	void PlaySound(FMOD::Sound* Sound, FMOD::Channel*& Channel, unsigned int Ms);
	void PauseSound(FMOD::Channel*& Channel, bool Flag);
	void StopSound(FMOD::Channel*& Channel);
	unsigned int GetLength(FMOD::Sound* Sound);
	unsigned int GetPlayTime(FMOD::Channel* Channel);
	void SetPlaySpeed(FMOD::Channel*& Channel, float Speed);
	void ResetPlaySpeed(FMOD::Channel*& Channel);
	void SetFreqCutOff(FMOD::Channel*& Channel, float Frequency);
	void SetBeatDetect(FMOD::Channel*& Channel);
	float DetectBeat(float Threshold, float SamplingRate);
	void UnSetBeatDetect(FMOD::Channel*& Channel);
	void UnSetFreqCutOff(FMOD::Channel*& Channel);
	size_t GetSoundNum();
	int GetSoundNumIf(std::string ContainedStr);
	void SetDistance(FMOD::Channel*& Channel, float MinDist, float MaxDist);
	void SetListnerPosition(float X, float Y);
	void SetSoundPosition(FMOD::Channel*& Channel, float X, float Y);
};
extern SoundUtil soundUtil;
