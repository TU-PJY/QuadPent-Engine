#include "SDK_Scene.h"
#include "SDK_SoundTool.h"
#include "SDK_EXTool.h"

MSDK::SDK_SoundTool MSDK::SoundTool;

void MSDK::SDK_SoundTool::Init() {
	Result = FMOD::System_Create(&SoundSystem);

	if (Result != FMOD_OK)	
		exit(EXIT_FAILURE);

	SoundSystem->init(MAX_CHANNEL_SIZE, FMOD_INIT_NORMAL, ExtDvData);
	SoundSystem->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &LowPass);
	SoundSystem->set3DSettings(1.0, 1.0, 2.0); 

	FFTdata.reserve(FFT_SIZE);
}

void MSDK::SDK_SoundTool::Load(MSDK::Sound& Sound, std::string FileName, FMOD_MODE Option) {
	HRESULT Result = SoundSystem->createSound(FileName.c_str(), Option, 0, &Sound);
	if (Result != FMOD_OK)
		MSDK::Scene.SetErrorScreen(ERROR_TYPE_AUDIO_LOAD, FileName);
}

void MSDK::SDK_SoundTool::Release(MSDK::Sound& Sound) {
	Sound->release();
}

void MSDK::SDK_SoundTool::Update() {
	SoundSystem->update();
}

void MSDK::SDK_SoundTool::SetFastSoundChannel(MSDK::FastSoundChannel& ChannelVar, int NumChannel) {
	ChannelVar.NumChannel = NumChannel;
	ChannelVar.Channel.assign(NumChannel, {});
	ChannelVar.PlayChannel = 0;
	ChannelVar.StopChannel = 1;
}

void MSDK::SDK_SoundTool::AddChannelToGroup(MSDK::SoundChannelGroup& Group, MSDK::Sound& Sound, MSDK::SoundChannel& ChannelVar) {
	SoundSystem->playSound(Sound, 0, true, &ChannelVar.Channel);
	Group.emplace_back(ChannelVar);
}

void MSDK::SDK_SoundTool::DeleteChannelFromGroup(MSDK::SoundChannelGroup& Group, MSDK::SoundChannel& ChannelVar) {
	for (auto It = begin(Group); It != end(Group);) {
		if (It->Channel == ChannelVar.Channel)
			It = Group.erase(It);
		else
			It++;
	}
}

void MSDK::SDK_SoundTool::ClearGroup(MSDK::SoundChannelGroup& Group) {
	Group.clear();
}

void MSDK::SDK_SoundTool::PlayGroup(MSDK::SoundChannelGroup& Group) {
	for (auto& G : Group) {
		G.Channel->setPaused(false);
		G.Channel->setVolume(G.Volume);
		G.Channel->setPitch(G.PlaySpeed);
	}
}

void MSDK::SDK_SoundTool::StopGroup(MSDK::SoundChannelGroup& Group) {
	for (auto& G : Group) {
		G.Channel->setPaused(true);
		G.Channel->setPosition(0, FMOD_TIMEUNIT_MS);
	}
}

void MSDK::SDK_SoundTool::SetGroupVolume(MSDK::SoundChannelGroup& Group, float Volume) {
	for (auto& G : Group) 
		G.Channel->setVolume(Volume);
}

void MSDK::SDK_SoundTool::SetGroupPlaySpeed(MSDK::SoundChannelGroup& Group, float Speed) {
	for (auto& G : Group)
		G.Channel->setPitch(Speed);
}

void MSDK::SDK_SoundTool::PauseGroup(MSDK::SoundChannelGroup& Group, bool Flag) {
	for (auto& G : Group)
		G.Channel->setPaused(Flag);
}

void MSDK::SDK_SoundTool::Play(MSDK::Sound& Sound, MSDK::SoundChannel& ChannelVar, float Time) {
	SoundSystem->playSound(Sound, 0, false, &ChannelVar.Channel);
	ChannelVar.Channel->setPosition(Time * 1000, FMOD_TIMEUNIT_MS);
	ChannelVar.Channel->setVolume(ChannelVar.Volume);
	ChannelVar.Channel->setPitch(ChannelVar.PlaySpeed);
}

void MSDK::SDK_SoundTool::Play(MSDK::Sound& Sound, MSDK::FastSoundChannel& ChannelVar, float Time) {
	SoundSystem->playSound(Sound, 0, false, &ChannelVar.Channel[ChannelVar.PlayChannel]);
	ChannelVar.Channel[ChannelVar.PlayChannel]->setPosition(Time * 1000, FMOD_TIMEUNIT_MS);
	ChannelVar.Channel[ChannelVar.PlayChannel]->setVolume(ChannelVar.Volume);
	ChannelVar.Channel[ChannelVar.PlayChannel++]->setPitch(ChannelVar.PlaySpeed);
	ChannelVar.Channel[ChannelVar.StopChannel++]->stop();

	MSDK::EXTool.ClampValue(ChannelVar.PlayChannel, 0, ChannelVar.NumChannel, CLAMP_RETURN);
	MSDK::EXTool.ClampValue(ChannelVar.StopChannel, 0, ChannelVar.NumChannel, CLAMP_RETURN);
}

void MSDK::SDK_SoundTool::PlayOnce(MSDK::Sound& Sound, MSDK::SoundChannel& ChannelVar, bool& BoolValue, float Time) {
	if (!BoolValue) {
		SoundSystem->playSound(Sound, 0, false, &ChannelVar.Channel);
		ChannelVar.Channel->setPosition(Time * 1000, FMOD_TIMEUNIT_MS);
		ChannelVar.Channel->setVolume(ChannelVar.Volume);
		ChannelVar.Channel->setPitch(ChannelVar.PlaySpeed);
	}
}

void MSDK::SDK_SoundTool::Pause(MSDK::SoundChannel& ChannelVar, bool Flag) {
	ChannelVar.Channel->setPaused(Flag);
}

void MSDK::SDK_SoundTool::Stop(MSDK::SoundChannel& ChannelVar) {
	ChannelVar.Channel->stop();
}

void MSDK::SDK_SoundTool::FadeOut(MSDK::SoundChannel& ChannelVar, float Speed, float FrameTime) {
	ChannelVar.Volume -= Speed * FrameTime;
	MSDK::EXTool.ClampValue(ChannelVar.Volume, 0.0, CLAMP_LESS);
	ChannelVar.Channel->setVolume(ChannelVar.Volume);
}

void MSDK::SDK_SoundTool::FadeIn(MSDK::SoundChannel& ChannelVar, float Speed, float FrameTime, float DestVolume) {
	ChannelVar.Volume += Speed * FrameTime;
	MSDK::EXTool.ClampValue(ChannelVar.Volume, DestVolume, CLAMP_GREATER);
	ChannelVar.Channel->setVolume(ChannelVar.Volume);
}

unsigned int MSDK::SDK_SoundTool::GetLength(MSDK::Sound& Sound) {
	unsigned int Length{};
	Sound->getLength(&Length, FMOD_TIMEUNIT_MS);
	return Length;
}

unsigned int MSDK::SDK_SoundTool::GetPlayTime(MSDK::SoundChannel& ChannelVar) {
	unsigned int Position{};
	ChannelVar.Channel->getPosition(&Position, FMOD_TIMEUNIT_MS);
	return Position;
}

void MSDK::SDK_SoundTool::SetVolume(MSDK::SoundChannel& ChannelVar, float Volume) {
	ChannelVar.Volume = Volume;
	ChannelVar.Channel->setVolume(Volume);
}

void MSDK::SDK_SoundTool::SetVolume(MSDK::FastSoundChannel& ChannelVar, float Volume) {
	ChannelVar.Volume = Volume;
	for(int i = 0; i < ChannelVar.NumChannel; i++)
		ChannelVar.Channel[i]->setVolume(Volume);
}

void MSDK::SDK_SoundTool::SetPlayTime(MSDK::SoundChannel& ChannelVar, float Time) {
	ChannelVar.Channel->setPosition(Time * 1000, FMOD_TIMEUNIT_MS);
}

void MSDK::SDK_SoundTool::SetPlaySpeed(MSDK::SoundChannel& ChannelVar, float Speed) {
	ChannelVar.PlaySpeed = Speed;
	ChannelVar.Channel->setPitch(Speed);
}

void MSDK::SDK_SoundTool::SetPlaySpeed(MSDK::FastSoundChannel& ChannelVar, float Speed) {
	ChannelVar.PlaySpeed = Speed;
	for (int i = 0; i < ChannelVar.NumChannel; i++)
		ChannelVar.Channel[i]->setPitch(Speed);
}

void MSDK::SDK_SoundTool::ResetPlaySpeed(MSDK::SoundChannel& ChannelVar) {
	ChannelVar.PlaySpeed = 1.0;
	ChannelVar.Channel->setPitch(1.0);
}

void MSDK::SDK_SoundTool::ResetPlaySpeed(MSDK::FastSoundChannel& ChannelVar) {
	ChannelVar.PlaySpeed = 1.0;
	for (int i = 0; i < ChannelVar.NumChannel; i++)
		ChannelVar.Channel[i]->setPitch(1.0);
}

void MSDK::SDK_SoundTool::SetFreqCutOff(MSDK::SoundChannel& ChannelVar, float Frequency) {
	LowPass->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, Frequency);
	ChannelVar.Channel->addDSP(0, LowPass);
}

void MSDK::SDK_SoundTool::SetBeatDetect(MSDK::SoundChannel& ChannelVar) {
	SoundSystem->createDSPByType(FMOD_DSP_TYPE_FFT, &BeatDetector);
	ChannelVar.Channel->addDSP(0, BeatDetector);
}

float MSDK::SDK_SoundTool::DetectBeat(float Threshold, int SamplingRate) {
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

void MSDK::SDK_SoundTool::DetectBeat(float& DestValue, float Threshold, int SamplingRate) {
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

bool MSDK::SDK_SoundTool::IsBeat(float Threshold, int SamplingRate) {
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

void MSDK::SDK_SoundTool::DisableBeatDetect(MSDK::SoundChannel& ChannelVar) {
	ChannelVar.Channel->removeDSP(BeatDetector);
}

void MSDK::SDK_SoundTool::DisableFreqCutOff(MSDK::SoundChannel& ChannelVar) {
	ChannelVar.Channel->removeDSP(LowPass);
}

void MSDK::SDK_SoundTool::StopAllSounds() {
	FMOD::ChannelGroup* MasterChannelGroup{};

	SoundSystem->getMasterChannelGroup(&MasterChannelGroup);
	Result = MasterChannelGroup->stop();
}

void MSDK::SDK_SoundTool::SetDistance(MSDK::SoundChannel& ChannelVar, float MinDist, float MaxDist) {
	ChannelVar.Channel->set3DMinMaxDistance(MinDist, MaxDist);
}

void MSDK::SDK_SoundTool::SetListnerPosition(float X, float Y) {
	ListenerPosition.x = X;
	ListenerPosition.y = Y;
	ListenerPosition.z = 0.0f;

	SoundSystem->set3DListenerAttributes(0, &ListenerPosition, 0, 0, 0);
};

void MSDK::SDK_SoundTool::SetListnerPosition(glm::vec2 Position) {
	ListenerPosition.x = Position.x;
	ListenerPosition.y = Position.y;
	ListenerPosition.z = 0.0f;

	SoundSystem->set3DListenerAttributes(0, &ListenerPosition, 0, 0, 0);
}

void MSDK::SDK_SoundTool::SetSoundPosition(MSDK::SoundChannel& ChannelVar, float X, float Y, float Diff) {
	SoundPosition.x = X;
	SoundPosition.y = Y;
	SoundPosition.z = Diff;

	ChannelVar.Channel->set3DAttributes(&SoundPosition, 0);
}

void MSDK::SDK_SoundTool::SetSoundPosition(MSDK::SoundChannel& ChannelVar, glm::vec2 Position, float Diff) {
	SoundPosition.x = Position.x;
	SoundPosition.y = Position.y;
	SoundPosition.z = Diff;

	ChannelVar.Channel->set3DAttributes(&SoundPosition, 0);
}