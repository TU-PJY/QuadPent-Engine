#include "Scene.h"
#include "SoundUtil.h"
#include "Config.h"
#include "EXUtil.h"

SDK_SoundTool SDK::SoundTool;

void SDK_SoundTool::Init() {
	Result = FMOD::System_Create(&SoundSystem);

	if (Result != FMOD_OK)	
		exit(EXIT_FAILURE);

	SoundSystem->init(MAX_CHANNEL_SIZE, FMOD_INIT_NORMAL, ExtDvData);
	SoundSystem->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &LowPass);
	SoundSystem->set3DSettings(1.0, 1.0, 2.0); 

	FFTdata.reserve(FFT_SIZE);
}

void SDK_SoundTool::Load(Sound& Sound, std::string FileName, FMOD_MODE Option) {
	HRESULT Result = SoundSystem->createSound(FileName.c_str(), Option, 0, &Sound);
	if (Result != FMOD_OK)
		SDK::Scene.SetErrorScreen(ERROR_TYPE_AUDIO_LOAD, FileName);
}

void SDK_SoundTool::Release(Sound& Sound) {
	Sound->release();
}

void SDK_SoundTool::Update() {
	SoundSystem->update();
}

void SDK_SoundTool::AddChannelToGroup(SoundChannelGroup& Group, Sound& Sound, SoundChannel& ChannelVar) {
	SoundSystem->playSound(Sound, 0, true, &ChannelVar.Channel);
	Group.emplace_back(ChannelVar);
}

void SDK_SoundTool::DeleteChannelFromGroup(SoundChannelGroup& Group, SoundChannel& ChannelVar) {
	for (auto It = begin(Group); It != end(Group);) {
		if (It->Channel == ChannelVar.Channel)
			It = Group.erase(It);
		else
			It++;
	}
}

void SDK_SoundTool::ClearGroup(SoundChannelGroup& Group) {
	Group.clear();
}

void SDK_SoundTool::PlayGroup(SoundChannelGroup& Group) {
	for (auto& G : Group) {
		G.Channel->setPaused(false);
		G.Channel->setVolume(G.Volume);
		G.Channel->setPitch(G.PlaySpeed);
	}
}

void SDK_SoundTool::StopGroup(SoundChannelGroup& Group) {
	for (auto& G : Group) {
		G.Channel->setPaused(true);
		G.Channel->setPosition(0, FMOD_TIMEUNIT_MS);
	}
}

void SDK_SoundTool::PauseGroup(SoundChannelGroup& Group, bool Flag) {
	for (auto& G : Group)
		G.Channel->setPaused(Flag);
}

void SDK_SoundTool::Play(Sound& Sound, SoundChannel& ChannelVar, float Time) {
	SoundSystem->playSound(Sound, 0, false, &ChannelVar.Channel);
	ChannelVar.Channel->setPosition(Time * 1000, FMOD_TIMEUNIT_MS);
	ChannelVar.Channel->setVolume(ChannelVar.Volume);
	ChannelVar.Channel->setPitch(ChannelVar.PlaySpeed);
}

void SDK_SoundTool::PlayOnce(Sound& Sound, SoundChannel& ChannelVar, bool& BoolValue, float Time) {
	if (!BoolValue) {
		SoundSystem->playSound(Sound, 0, false, &ChannelVar.Channel);
		ChannelVar.Channel->setPosition(Time * 1000, FMOD_TIMEUNIT_MS);
		ChannelVar.Channel->setVolume(ChannelVar.Volume);
		ChannelVar.Channel->setPitch(ChannelVar.PlaySpeed);
	}
}

void SDK_SoundTool::Pause(SoundChannel& ChannelVar, bool Flag) {
	ChannelVar.Channel->setPaused(Flag);
}

void SDK_SoundTool::Stop(SoundChannel& ChannelVar) {
	ChannelVar.Channel->stop();
}

void SDK_SoundTool::FadeOut(SoundChannel& ChannelVar, float Speed, float FrameTime) {
	ChannelVar.Volume -= Speed * FrameTime;
	SDK::EXTool.ClampValue(ChannelVar.Volume, 0.0, CLAMP_LESS);
	ChannelVar.Channel->setVolume(ChannelVar.Volume);
}

void SDK_SoundTool::FadeIn(SoundChannel& ChannelVar, float Speed, float FrameTime, float DestVolume) {
	ChannelVar.Volume += Speed * FrameTime;
	SDK::EXTool.ClampValue(ChannelVar.Volume, DestVolume, CLAMP_GREATER);
	ChannelVar.Channel->setVolume(ChannelVar.Volume);
}

unsigned int SDK_SoundTool::GetLength(Sound& Sound) {
	unsigned int Length{};
	Sound->getLength(&Length, FMOD_TIMEUNIT_MS);
	return Length;
}

unsigned int SDK_SoundTool::GetPlayTime(SoundChannel& ChannelVar) {
	unsigned int Position{};
	ChannelVar.Channel->getPosition(&Position, FMOD_TIMEUNIT_MS);
	return Position;
}

void SDK_SoundTool::SetVolume(SoundChannel& ChannelVar, float Volume) {
	ChannelVar.Volume = Volume;
	ChannelVar.Channel->setVolume(Volume);
}

void SDK_SoundTool::SetPlayTime(SoundChannel& ChannelVar, float Time) {
	ChannelVar.Channel->setPosition(Time * 1000, FMOD_TIMEUNIT_MS);
}

void SDK_SoundTool::SetPlaySpeed(SoundChannel& ChannelVar, float Speed) {
	ChannelVar.PlaySpeed = Speed;
	ChannelVar.Channel->setPitch(Speed);
}

void SDK_SoundTool::ResetPlaySpeed(SoundChannel& ChannelVar) {
	ChannelVar.PlaySpeed = 1.0;
	ChannelVar.Channel->setPitch(1.0);
}

void SDK_SoundTool::SetFreqCutOff(SoundChannel& ChannelVar, float Frequency) {
	LowPass->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, Frequency);
	ChannelVar.Channel->addDSP(0, LowPass);
}

void SDK_SoundTool::SetBeatDetect(SoundChannel& ChannelVar) {
	SoundSystem->createDSPByType(FMOD_DSP_TYPE_FFT, &BeatDetector);
	ChannelVar.Channel->addDSP(0, BeatDetector);
}

float SDK_SoundTool::DetectBeat(float Threshold, int SamplingRate) {
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

void SDK_SoundTool::DetectBeat(float& DestValue, float Threshold, int SamplingRate) {
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

bool SDK_SoundTool::IsBeat(float Threshold, int SamplingRate) {
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

void SDK_SoundTool::DisableBeatDetect(SoundChannel& ChannelVar) {
	ChannelVar.Channel->removeDSP(BeatDetector);
}

void SDK_SoundTool::DisableFreqCutOff(SoundChannel& ChannelVar) {
	ChannelVar.Channel->removeDSP(LowPass);
}

void SDK_SoundTool::StopAllSounds() {
	FMOD::ChannelGroup* MasterChannelGroup{};

	SoundSystem->getMasterChannelGroup(&MasterChannelGroup);
	Result = MasterChannelGroup->stop();
}

void SDK_SoundTool::SetDistance(SoundChannel& ChannelVar, float MinDist, float MaxDist) {
	ChannelVar.Channel->set3DMinMaxDistance(MinDist, MaxDist);
}

void SDK_SoundTool::SetListnerPosition(float X, float Y) {
	ListenerPosition.x = X;
	ListenerPosition.y = Y;
	ListenerPosition.z = 0.0f;

	SoundSystem->set3DListenerAttributes(0, &ListenerPosition, 0, 0, 0);
};

void SDK_SoundTool::SetListnerPosition(glm::vec2 Position) {
	ListenerPosition.x = Position.x;
	ListenerPosition.y = Position.y;
	ListenerPosition.z = 0.0f;

	SoundSystem->set3DListenerAttributes(0, &ListenerPosition, 0, 0, 0);
}

void SDK_SoundTool::SetSoundPosition(SoundChannel& ChannelVar, float X, float Y, float Diff) {
	SoundPosition.x = X;
	SoundPosition.y = Y;
	SoundPosition.z = Diff;

	ChannelVar.Channel->set3DAttributes(&SoundPosition, 0);
}

void SDK_SoundTool::SetSoundPosition(SoundChannel& ChannelVar, glm::vec2 Position, float Diff) {
	SoundPosition.x = Position.x;
	SoundPosition.y = Position.y;
	SoundPosition.z = Diff;

	ChannelVar.Channel->set3DAttributes(&SoundPosition, 0);
}