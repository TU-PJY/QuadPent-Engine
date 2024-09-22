#pragma once
#include "EngineHeader.h"
#include <map>
#include <string>

#define FFT_SIZE 1024 // FFT Size

class SoundUtil {
private:
	FMOD::System* SoundSystem{};
	FMOD::DSP* BeatDetector{};
	FMOD::DSP* LowPass{};

	FMOD_VECTOR listenerPos{};
	FMOD_VECTOR soundPos{};

	FMOD_RESULT Result{};
	void* ExtDvData{};

public:
	void Init();
	void Import(Sound& Sound, const char* FileName, FMOD_MODE Option);
	void Release(Sound& Sound);
	void Update();
	void PlaySound(FMOD::Sound* Sound, FMOD::Channel*& Channel, unsigned int Ms=0);
	void PauseSound(FMOD::Channel*& Channel, bool Flag);
	void StopSound(FMOD::Channel*& Channel);
	unsigned int GetLength(FMOD::Sound* Sound);
	unsigned int GetPlayTime(FMOD::Channel* Channel);
	void SetPlaySpeed(FMOD::Channel*& Channel, float Speed);
	void ResetPlaySpeed(FMOD::Channel*& Channel);
	void SetFreqCutOff(FMOD::Channel*& Channel, float Frequency);
	void SetBeatDetect(FMOD::Channel*& Channel);
	float DetectBeat(float Threshold, float SamplingRate);
	bool IsBeat(float Threshold, float SamplingRate);
	void UnSetBeatDetect(FMOD::Channel*& Channel);
	void UnSetFreqCutOff(FMOD::Channel*& Channel);
	void SetDistance(FMOD::Channel*& Channel, float MinDist, float MaxDist);
	void SetListnerPosition(float X, float Y);
	void SetSoundPosition(FMOD::Channel*& Channel, float X, float Y, float Diff);
};
extern SoundUtil soundUtil;
