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

	FMOD_RESULT Result{};
	void* ExtDvData{};

public:
	void Init();
	void LoadSoundFromList();
	void LoadChannelFromList();
	void Update();
	unsigned int GetLength(std::string SoundName);
	void PlaySound(std::string SoundName, std::string ChannelName, unsigned int Ms=0);
	void PauseSound(std::string ChannelName, bool Flag);
	void StopSound(std::string ChannelName);
	unsigned int GetPlayTime(std::string ChannelName);
	void SetPlaySpeed(std::string ChannelName, float PlaySpeed);
	void SetFreqCutOff(std::string ChannelName, float Frequency);
	void UnSetFreqCutOff(std::string ChannelName);
	void SetBeatDetect(std::string ChannelName);
	void UnSetBeatDetect(std::string ChannelName);
	float DetectBeat(float Threshold, int DataSize);
	size_t GetSoundNum();
	int GetSoundNumIf(std::string ContainedStr);
};
extern SoundUtil soundUtil;
