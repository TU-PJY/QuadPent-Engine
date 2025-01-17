#include "Scene.h"
#include "SoundUtil.h"

#ifdef USE_SOUND_SYSTEM
#include <vector>
#include "Config.h"

SoundUtil soundUtil;

std::vector<float> FFTdata(FFT_SIZE);

void SoundUtil::Init() {
	Result = FMOD::System_Create(&SoundSystem);

	if (Result != FMOD_OK)	
		exit(EXIT_FAILURE);

	SoundSystem->init(MAX_CHANNEL_SIZE, FMOD_INIT_NORMAL, ExtDvData);
	SoundSystem->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &LowPass);
	SoundSystem->set3DSettings(1.0, 1.0, 2.0); 
}

void SoundUtil::Load(FMOD::Sound*& Sound, std::string FileName, FMOD_MODE Option) {
	HRESULT Result = SoundSystem->createSound(FileName.c_str(), Option, 0, &Sound);
	if (Result != FMOD_OK)
		scene.ErrorScreen(ERROR_TYPE_AUDIO_LOAD, FileName);
}

void SoundUtil::Release(FMOD::Sound*& Sound) {
	Sound->release();
}

void SoundUtil::Update() {
	SoundSystem->update();
}

void SoundUtil::PlaySound(FMOD::Sound* Sound, FMOD::Channel*& ChannelVar, unsigned int Ms) {
	SoundSystem->playSound(Sound, 0, false, &ChannelVar);
	if (Ms > 0)
		ChannelVar->setPosition(Ms, FMOD_TIMEUNIT_MS);
}

void SoundUtil::PlaySoundOnce(FMOD::Sound* Sound, FMOD::Channel*& ChannelVar, bool& FlagValue, unsigned int Ms) {
	if (FlagValue) {
		SoundSystem->playSound(Sound, 0, false, &ChannelVar);
		FlagValue = false;
	}
}

void SoundUtil::PauseSound(FMOD::Channel*& ChannelVar, bool Flag) {
	ChannelVar->setPaused(Flag);
}

void SoundUtil::StopSound(FMOD::Channel*& ChannelVar) {
	ChannelVar->stop();
}

unsigned int SoundUtil::GetLength(FMOD::Sound* Sound) {
	unsigned int Length{};
	Sound->getLength(&Length, FMOD_TIMEUNIT_MS);
	return Length;
}

unsigned int SoundUtil::GetPlayTime(FMOD::Channel* ChannelVar) {
	unsigned int Position{};
	ChannelVar->getPosition(&Position, FMOD_TIMEUNIT_MS);
	return Position;
}

void SoundUtil::SetPlaySpeed(FMOD::Channel*& ChannelVar, float Speed) {
	ChannelVar->setPitch(Speed);
}

void SoundUtil::ResetPlaySpeed(FMOD::Channel*& ChannelVar) {
	ChannelVar->setPitch(1.0);
}

void SoundUtil::SetFreqCutOff(FMOD::Channel*& ChannelVar, float Frequency) {
	LowPass->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, Frequency);
	ChannelVar->addDSP(0, LowPass);
}

void SoundUtil::SetBeatDetect(FMOD::Channel*& ChannelVar) {
	SoundSystem->createDSPByType(FMOD_DSP_TYPE_FFT, &BeatDetector);
	ChannelVar->addDSP(0, BeatDetector);
}

float SoundUtil::DetectBeat(float Threshold, float SamplingRate) {
	FMOD_DSP_PARAMETER_FFT* FFT = nullptr;
	BeatDetector->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, (void**)&FFT, 0, 0, 0);

	if (FFT) {
		int NumChannels = FFT->numchannels;

		if (NumChannels > 0) {
			memcpy(FFTdata.data(), FFT->spectrum[0], FFT_SIZE * sizeof(float));

			float BassEnergy = 0.0f;

			for (int i = 0; i < SamplingRate; ++i)
				BassEnergy += FFTdata[i];

			if (BassEnergy > Threshold)
				return BassEnergy;
		}
	}

	return 0;
}

bool SoundUtil::IsBeat(float Threshold, float SamplingRate) {
	FMOD_DSP_PARAMETER_FFT* FFT = nullptr;
	BeatDetector->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, (void**)&FFT, 0, 0, 0);

	if (FFT) {
		int NumChannels = FFT->numchannels;

		if (NumChannels > 0) {
			memcpy(FFTdata.data(), FFT->spectrum[0], FFT_SIZE * sizeof(float));

			float BassEnergy = 0.0f;

			for (int i = 0; i < SamplingRate; ++i)
				BassEnergy += FFTdata[i];

			if (BassEnergy > Threshold)
				return true;
		}
	}

	return false;
}

void SoundUtil::UnSetBeatDetect(FMOD::Channel*& ChannelVar) {
	ChannelVar->removeDSP(BeatDetector);
}

void SoundUtil::UnSetFreqCutOff(FMOD::Channel*& ChannelVar) {
	ChannelVar->removeDSP(LowPass);
}

void SoundUtil::SetDistance(FMOD::Channel*& ChannelVar, float MinDist, float MaxDist) {
	ChannelVar->set3DMinMaxDistance(MinDist, MaxDist);
}

void SoundUtil::SetListnerPosition(float X, float Y) {
	ListenerPosition.x = X;
	ListenerPosition.y = Y;
	ListenerPosition.z = 0.0f;

	SoundSystem->set3DListenerAttributes(0, &ListenerPosition, 0, 0, 0);
};

void SoundUtil::SetListnerPosition(glm::vec2 Position) {
	ListenerPosition.x = Position.x;
	ListenerPosition.y = Position.y;
	ListenerPosition.z = 0.0f;

	SoundSystem->set3DListenerAttributes(0, &ListenerPosition, 0, 0, 0);
}

void SoundUtil::SetSoundPosition(FMOD::Channel*& ChannelVar, float X, float Y, float Diff) {
	SoundPosition.x = X;
	SoundPosition.y = Y;
	SoundPosition.z = Diff;

	ChannelVar->set3DAttributes(&SoundPosition, 0);
}

void SoundUtil::SetSoundPosition(FMOD::Channel*& ChannelVar, glm::vec2 Position, float Diff) {
	SoundPosition.x = Position.x;
	SoundPosition.y = Position.y;
	SoundPosition.z = Diff;

	ChannelVar->set3DAttributes(&SoundPosition, 0);
}

void SoundUtil::StopAllSounds() {
	FMOD::ChannelGroup* MasterChannelGroup{};

	FMOD_RESULT Result = SoundSystem->getMasterChannelGroup(&MasterChannelGroup);
	if (Result != FMOD_OK) {
		printf("FMOD error! (%d) %s\n", Result, FMOD_ErrorString(Result));
		return;
	}

	Result = MasterChannelGroup->stop();
	if (Result != FMOD_OK)
		printf("FMOD error! (%d) %s\n", Result, FMOD_ErrorString(Result));
}
#endif