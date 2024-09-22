#include "SoundUtil.h"
#include <vector>

std::vector<float> FFTdata(FFT_SIZE);

void SoundUtil::Init() {
	Result = FMOD::System_Create(&SoundSystem);

	if (Result != FMOD_OK)	
		exit(EXIT_FAILURE);

	SoundSystem->init(32, FMOD_INIT_NORMAL, ExtDvData);
	SoundSystem->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &LowPass);
	SoundSystem->set3DSettings(1.0, 1.0, 2.0); 
}

void SoundUtil::ImportSound(Sound& Sound, const char* FileName, FMOD_MODE Option) {
	SoundSystem->createSound(FileName, Option, 0, &Sound);
}

void SoundUtil::ReleaseSound(Sound& Sound) {
	Sound->release();
}

void SoundUtil::Update() {
	SoundSystem->update();
}

void SoundUtil::PlaySound(FMOD::Sound* Sound, FMOD::Channel*& Channel, unsigned int Ms) {
	SoundSystem->playSound(Sound, 0, false, &Channel);
	if (Ms > 0)
		Channel->setPosition(Ms, FMOD_TIMEUNIT_MS);
}

void SoundUtil::PauseSound(FMOD::Channel*& Channel, bool Flag) {
	Channel->setPaused(Flag);
}

void SoundUtil::StopSound(FMOD::Channel*& Channel) {
	Channel->stop();
}

unsigned int SoundUtil::GetLength(FMOD::Sound* Sound) {
	unsigned int Length{};
	Sound->getLength(&Length, FMOD_TIMEUNIT_MS);
	return Length;
}

unsigned int SoundUtil::GetPlayTime(FMOD::Channel* Channel) {
	unsigned int Position{};
	Channel->getPosition(&Position, FMOD_TIMEUNIT_MS);
	return Position;
}

void SoundUtil::SetPlaySpeed(FMOD::Channel*& Channel, float Speed) {
	Channel->setPitch(Speed);
}

void SoundUtil::ResetPlaySpeed(FMOD::Channel*& Channel) {
	Channel->setPitch(1.0);
}

void SoundUtil::SetFreqCutOff(FMOD::Channel*& Channel, float Frequency) {
	LowPass->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, Frequency);
	Channel->addDSP(0, LowPass);
}

void SoundUtil::SetBeatDetect(FMOD::Channel*& Channel) {
	SoundSystem->createDSPByType(FMOD_DSP_TYPE_FFT, &BeatDetector);
	Channel->addDSP(0, BeatDetector);
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

void SoundUtil::UnSetBeatDetect(FMOD::Channel*& Channel) {
	Channel->removeDSP(BeatDetector);
}

void SoundUtil::UnSetFreqCutOff(FMOD::Channel*& Channel) {
	Channel->removeDSP(LowPass);
}

void SoundUtil::SetDistance(FMOD::Channel*& Channel, float MinDist, float MaxDist) {
	Channel->set3DMinMaxDistance(MinDist, MaxDist);
}

void SoundUtil::SetListnerPosition(float X, float Y) {
	listenerPos.x = X;
	listenerPos.y = Y;
	listenerPos.z = 0.0f;

	SoundSystem->set3DListenerAttributes(0, &listenerPos, 0, 0, 0);
};

void SoundUtil::SetSoundPosition(FMOD::Channel*& Channel, float X, float Y, float Diff) {
	soundPos.x = X;
	soundPos.y = Y;
	soundPos.z = Diff;

	Channel->set3DAttributes(&soundPos, 0);
}