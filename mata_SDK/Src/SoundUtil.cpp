#include "Scene.h"
#include "SoundUtil.h"
#include "Config.h"
#include "EXUtil.h"

SoundUtil SDK::SoundTool;

void SoundUtil::Init() {
	Result = FMOD::System_Create(&SoundSystem);

	if (Result != FMOD_OK)	
		exit(EXIT_FAILURE);

	SoundSystem->init(MAX_CHANNEL_SIZE, FMOD_INIT_NORMAL, ExtDvData);
	SoundSystem->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &LowPass);
	SoundSystem->set3DSettings(1.0, 1.0, 2.0); 

	FFTdata.reserve(FFT_SIZE);
}

void SoundUtil::Load(Sound& Sound, std::string FileName, FMOD_MODE Option) {
	HRESULT Result = SoundSystem->createSound(FileName.c_str(), Option, 0, &Sound);
	if (Result != FMOD_OK)
		scene.SetErrorScreen(ERROR_TYPE_AUDIO_LOAD, FileName);
}

void SoundUtil::Release(Sound& Sound) {
	Sound->release();
}

void SoundUtil::Update() {
	SoundSystem->update();
}

void SoundUtil::AddChannelToGroup(SoundChannelGroup& Group, Sound& Sound, SoundChannel& ChannelVar) {
	SoundSystem->playSound(Sound, 0, true, &ChannelVar.Channel);
	Group.emplace_back(ChannelVar);
}

void SoundUtil::DeleteChannelFromGroup(SoundChannelGroup& Group, SoundChannel& ChannelVar) {
	for (auto It = begin(Group); It != end(Group);) {
		if (It->Channel == ChannelVar.Channel)
			It = Group.erase(It);
		else
			It++;
	}
}

void SoundUtil::ClearGroup(SoundChannelGroup& Group) {
	Group.clear();
}

void SoundUtil::PlayGroup(SoundChannelGroup& Group) {
	for (auto& G : Group) {
		G.Channel->setPaused(false);
		G.Channel->setVolume(G.Volume);
		G.Channel->setPitch(G.PlaySpeed);
	}
}

void SoundUtil::StopGroup(SoundChannelGroup& Group) {
	for (auto& G : Group) {
		G.Channel->setPaused(true);
		G.Channel->setPosition(0, FMOD_TIMEUNIT_MS);
	}
}

void SoundUtil::PauseGroup(SoundChannelGroup& Group, bool Flag) {
	for (auto& G : Group)
		G.Channel->setPaused(Flag);
}

void SoundUtil::Play(Sound& Sound, SoundChannel& ChannelVar, float Time) {
	SoundSystem->playSound(Sound, 0, false, &ChannelVar.Channel);
	ChannelVar.Channel->setPosition(Time * 1000, FMOD_TIMEUNIT_MS);
	ChannelVar.Channel->setVolume(ChannelVar.Volume);
	ChannelVar.Channel->setPitch(ChannelVar.PlaySpeed);
}

void SoundUtil::PlayOnce(Sound& Sound, SoundChannel& ChannelVar, bool& BoolValue, float Time) {
	if (!BoolValue) {
		SoundSystem->playSound(Sound, 0, false, &ChannelVar.Channel);
		ChannelVar.Channel->setPosition(Time * 1000, FMOD_TIMEUNIT_MS);
		ChannelVar.Channel->setVolume(ChannelVar.Volume);
		ChannelVar.Channel->setPitch(ChannelVar.PlaySpeed);
	}
}

void SoundUtil::Pause(SoundChannel& ChannelVar, bool Flag) {
	ChannelVar.Channel->setPaused(Flag);
}

void SoundUtil::Stop(SoundChannel& ChannelVar) {
	ChannelVar.Channel->stop();
}

void SoundUtil::FadeOut(SoundChannel& ChannelVar, float Speed, float FrameTime) {
	ChannelVar.Volume -= Speed * FrameTime;
	SDK::EXTool.ClampValue(ChannelVar.Volume, 0.0, CLAMP_LESS);
	ChannelVar.Channel->setVolume(ChannelVar.Volume);
}

void SoundUtil::FadeIn(SoundChannel& ChannelVar, float Speed, float FrameTime, float DestVolume) {
	ChannelVar.Volume += Speed * FrameTime;
	SDK::EXTool.ClampValue(ChannelVar.Volume, DestVolume, CLAMP_GREATER);
	ChannelVar.Channel->setVolume(ChannelVar.Volume);
}

unsigned int SoundUtil::GetLength(Sound& Sound) {
	unsigned int Length{};
	Sound->getLength(&Length, FMOD_TIMEUNIT_MS);
	return Length;
}

unsigned int SoundUtil::GetPlayTime(SoundChannel& ChannelVar) {
	unsigned int Position{};
	ChannelVar.Channel->getPosition(&Position, FMOD_TIMEUNIT_MS);
	return Position;
}

void SoundUtil::SetVolume(SoundChannel& ChannelVar, float Volume) {
	ChannelVar.Volume = Volume;
	ChannelVar.Channel->setVolume(Volume);
}

void SoundUtil::SetPlayTime(SoundChannel& ChannelVar, float Time) {
	ChannelVar.Channel->setPosition(Time * 1000, FMOD_TIMEUNIT_MS);
}

void SoundUtil::SetPlaySpeed(SoundChannel& ChannelVar, float Speed) {
	ChannelVar.PlaySpeed = Speed;
	ChannelVar.Channel->setPitch(Speed);
}

void SoundUtil::ResetPlaySpeed(SoundChannel& ChannelVar) {
	ChannelVar.PlaySpeed = 1.0;
	ChannelVar.Channel->setPitch(1.0);
}

void SoundUtil::SetFreqCutOff(SoundChannel& ChannelVar, float Frequency) {
	LowPass->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, Frequency);
	ChannelVar.Channel->addDSP(0, LowPass);
}

void SoundUtil::SetBeatDetect(SoundChannel& ChannelVar) {
	SoundSystem->createDSPByType(FMOD_DSP_TYPE_FFT, &BeatDetector);
	ChannelVar.Channel->addDSP(0, BeatDetector);
}

float SoundUtil::DetectBeat(float Threshold, int SamplingRate) {
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

void SoundUtil::DetectBeat(float& DestValue, float Threshold, int SamplingRate) {
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
				DestValue = BassEnergy;
		}
	}

	DestValue = 0.0;
}

bool SoundUtil::IsBeat(float Threshold, int SamplingRate) {
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

void SoundUtil::DisableBeatDetect(SoundChannel& ChannelVar) {
	ChannelVar.Channel->removeDSP(BeatDetector);
}

void SoundUtil::DisableFreqCutOff(SoundChannel& ChannelVar) {
	ChannelVar.Channel->removeDSP(LowPass);
}

void SoundUtil::StopAllSounds() {
	FMOD::ChannelGroup* MasterChannelGroup{};

	SoundSystem->getMasterChannelGroup(&MasterChannelGroup);
	Result = MasterChannelGroup->stop();
}

void SoundUtil::SetDistance(SoundChannel& ChannelVar, float MinDist, float MaxDist) {
	ChannelVar.Channel->set3DMinMaxDistance(MinDist, MaxDist);
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

void SoundUtil::SetSoundPosition(SoundChannel& ChannelVar, float X, float Y, float Diff) {
	SoundPosition.x = X;
	SoundPosition.y = Y;
	SoundPosition.z = Diff;

	ChannelVar.Channel->set3DAttributes(&SoundPosition, 0);
}

void SoundUtil::SetSoundPosition(SoundChannel& ChannelVar, glm::vec2 Position, float Diff) {
	SoundPosition.x = Position.x;
	SoundPosition.y = Position.y;
	SoundPosition.z = Diff;

	ChannelVar.Channel->set3DAttributes(&SoundPosition, 0);
}