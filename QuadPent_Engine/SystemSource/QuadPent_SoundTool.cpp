#include "QuadPent_Scene.h"
#include "QuadPent_SoundTool.h"
#include "QuadPent_EXTool.h"

QP::QuadPent_SoundTool QP::SoundTool;

void QP::QuadPent_SoundTool::Init() {
	Result = FMOD::System_Create(&SoundSystem);

	if (Result != FMOD_OK)	
		exit(EXIT_FAILURE);

	SoundSystem->init(MAX_CHANNEL_SIZE, FMOD_INIT_NORMAL, ExtDvData);
	SoundSystem->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &LowPass);
	SoundSystem->set3DSettings(1.0, 1.0, 2.0); 
	FFTdata.reserve(FFT_SIZE);

	FMOD::ChannelGroup* MasterGroup;
	SoundSystem->getMasterChannelGroup(&MasterGroup);
	float Volume;
	MasterGroup->getVolume(&Volume);
	if (Volume == 0.0f)
		MasterGroup->setVolume(1.0f);
}

void QP::QuadPent_SoundTool::Load(QP::Sound& Sound, std::string FileName, FMOD_MODE Option) {
	HRESULT Result = SoundSystem->createSound(FileName.c_str(), Option, 0, &Sound);
	if (Result != FMOD_OK)
		QP::Scene.SetErrorScreen(ERROR_TYPE_SOUND_LOAD, FileName);
}

void QP::QuadPent_SoundTool::Release(QP::Sound& Sound) {
	Sound->release();
}

void QP::QuadPent_SoundTool::Update() {
	SoundSystem->update();
}

void QP::QuadPent_SoundTool::SetFastSoundChannel(QP::FastSoundChannel& ChannelVar, int NumChannel) {
	ChannelVar.NumChannel = NumChannel;
	ChannelVar.Channel.assign(NumChannel, {});
	ChannelVar.PlayChannel = 0;
	ChannelVar.StopChannel = 1;
}

void QP::QuadPent_SoundTool::AddChannelToGroup(QP::SoundChannelGroup& Group, QP::Sound& Sound, QP::SoundChannel& ChannelVar) {
	SoundSystem->playSound(Sound, 0, true, &ChannelVar.Channel);
	Group.emplace_back(ChannelVar);
}

void QP::QuadPent_SoundTool::DeleteChannelFromGroup(QP::SoundChannelGroup& Group, QP::SoundChannel& ChannelVar) {
	for (auto It = begin(Group); It != end(Group);) {
		if (It->Channel == ChannelVar.Channel)
			It = Group.erase(It);
		else
			It++;
	}
}

void QP::QuadPent_SoundTool::ClearGroup(QP::SoundChannelGroup& Group) {
	Group.clear();
}

void QP::QuadPent_SoundTool::PlayGroup(QP::SoundChannelGroup& Group) {
	for (auto& G : Group) {
		G.Channel->setPaused(false);
		G.Channel->setVolume(G.Volume);
		G.Channel->setPitch(G.PlaySpeed);
	}
}

void QP::QuadPent_SoundTool::StopGroup(QP::SoundChannelGroup& Group) {
	for (auto& G : Group) {
		G.Channel->setPaused(true);
		G.Channel->setPosition(0, FMOD_TIMEUNIT_MS);
	}
}

void QP::QuadPent_SoundTool::SetGroupVolume(QP::SoundChannelGroup& Group, float Volume) {
	for (auto& G : Group) 
		G.Channel->setVolume(Volume);
}

void QP::QuadPent_SoundTool::SetGroupPlaySpeed(QP::SoundChannelGroup& Group, float Speed) {
	for (auto& G : Group)
		G.Channel->setPitch(Speed);
}

void QP::QuadPent_SoundTool::PauseGroup(QP::SoundChannelGroup& Group, bool Flag) {
	for (auto& G : Group)
		G.Channel->setPaused(Flag);
}

void QP::QuadPent_SoundTool::Play(QP::Sound& Sound, QP::SoundChannel& ChannelVar, float Time) {
	SoundSystem->playSound(Sound, 0, false, &ChannelVar.Channel);
	ChannelVar.Channel->setPosition(Time * 1000, FMOD_TIMEUNIT_MS);
	ChannelVar.Channel->setVolume(ChannelVar.Volume);
	ChannelVar.Channel->setPitch(ChannelVar.PlaySpeed);
}

void QP::QuadPent_SoundTool::Play(QP::Sound& Sound, QP::FastSoundChannel& ChannelVar, float Time) {
	SoundSystem->playSound(Sound, 0, false, &ChannelVar.Channel[ChannelVar.PlayChannel]);
	ChannelVar.Channel[ChannelVar.PlayChannel]->setPosition(Time * 1000, FMOD_TIMEUNIT_MS);
	ChannelVar.Channel[ChannelVar.PlayChannel]->setVolume(ChannelVar.Volume);
	ChannelVar.Channel[ChannelVar.PlayChannel++]->setPitch(ChannelVar.PlaySpeed);
	ChannelVar.Channel[ChannelVar.StopChannel++]->stop();

	QP::EXTool.ClampValue(ChannelVar.PlayChannel, 0, ChannelVar.NumChannel, CLAMP_RETURN);
	QP::EXTool.ClampValue(ChannelVar.StopChannel, 0, ChannelVar.NumChannel, CLAMP_RETURN);
}

void QP::QuadPent_SoundTool::PlayOnce(QP::Sound& Sound, QP::SoundChannel& ChannelVar, bool& BoolValue, float Time) {
	if (!BoolValue) {
		SoundSystem->playSound(Sound, 0, false, &ChannelVar.Channel);
		ChannelVar.Channel->setPosition(Time * 1000, FMOD_TIMEUNIT_MS);
		ChannelVar.Channel->setVolume(ChannelVar.Volume);
		ChannelVar.Channel->setPitch(ChannelVar.PlaySpeed);
		BoolValue = true;
	}
}

void QP::QuadPent_SoundTool::Pause(QP::SoundChannel& ChannelVar, bool Flag) {
	ChannelVar.Channel->setPaused(Flag);
}

void QP::QuadPent_SoundTool::Stop(QP::SoundChannel& ChannelVar) {
	ChannelVar.Channel->stop();
}

void QP::QuadPent_SoundTool::FadeOut(QP::SoundChannel& ChannelVar, float Speed, float FrameTime) {
	ChannelVar.Volume -= Speed * FrameTime;
	QP::EXTool.ClampValue(ChannelVar.Volume, 0.0, CLAMP_LESS);
	ChannelVar.Channel->setVolume(ChannelVar.Volume);
}

void QP::QuadPent_SoundTool::FadeIn(QP::SoundChannel& ChannelVar, float Speed, float FrameTime, float DestVolume) {
	ChannelVar.Volume += Speed * FrameTime;
	QP::EXTool.ClampValue(ChannelVar.Volume, DestVolume, CLAMP_GREATER);
	ChannelVar.Channel->setVolume(ChannelVar.Volume);
}

unsigned int QP::QuadPent_SoundTool::GetLength(QP::Sound& Sound) {
	unsigned int Length{};
	Sound->getLength(&Length, FMOD_TIMEUNIT_MS);
	return Length;
}

unsigned int QP::QuadPent_SoundTool::GetPlayTime(QP::SoundChannel& ChannelVar) {
	unsigned int Position{};
	ChannelVar.Channel->getPosition(&Position, FMOD_TIMEUNIT_MS);
	return Position;
}

void QP::QuadPent_SoundTool::SetVolume(QP::SoundChannel& ChannelVar, float Volume) {
	ChannelVar.Volume = Volume;
	ChannelVar.Channel->setVolume(Volume);
}

void QP::QuadPent_SoundTool::SetVolume(QP::FastSoundChannel& ChannelVar, float Volume) {
	ChannelVar.Volume = Volume;
	for(int i = 0; i < ChannelVar.NumChannel; i++)
		ChannelVar.Channel[i]->setVolume(Volume);
}

void QP::QuadPent_SoundTool::SetPlayTime(QP::SoundChannel& ChannelVar, float Time) {
	ChannelVar.Channel->setPosition(Time * 1000, FMOD_TIMEUNIT_MS);
}

void QP::QuadPent_SoundTool::SetPlaySpeed(QP::SoundChannel& ChannelVar, float Speed) {
	ChannelVar.PlaySpeed = Speed;
	ChannelVar.Channel->setPitch(Speed);
}

void QP::QuadPent_SoundTool::SetPlaySpeed(QP::FastSoundChannel& ChannelVar, float Speed) {
	ChannelVar.PlaySpeed = Speed;
	for (int i = 0; i < ChannelVar.NumChannel; i++)
		ChannelVar.Channel[i]->setPitch(Speed);
}

void QP::QuadPent_SoundTool::ResetPlaySpeed(QP::SoundChannel& ChannelVar) {
	ChannelVar.PlaySpeed = 1.0;
	ChannelVar.Channel->setPitch(1.0);
}

void QP::QuadPent_SoundTool::ResetPlaySpeed(QP::FastSoundChannel& ChannelVar) {
	ChannelVar.PlaySpeed = 1.0;
	for (int i = 0; i < ChannelVar.NumChannel; i++)
		ChannelVar.Channel[i]->setPitch(1.0);
}

void QP::QuadPent_SoundTool::SetFreqCutOff(QP::SoundChannel& ChannelVar, float Frequency) {
	LowPass->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, Frequency);
	ChannelVar.Channel->addDSP(0, LowPass);
}

void QP::QuadPent_SoundTool::SetBeatDetect(QP::SoundChannel& ChannelVar) {
	SoundSystem->createDSPByType(FMOD_DSP_TYPE_FFT, &BeatDetector);
	ChannelVar.Channel->addDSP(0, BeatDetector);
}

float QP::QuadPent_SoundTool::DetectBeat(float Threshold, int SamplingRate) {
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

void QP::QuadPent_SoundTool::DetectBeat(float& DestValue, float Threshold, int SamplingRate) {
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

bool QP::QuadPent_SoundTool::IsBeat(float Threshold, int SamplingRate) {
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

void QP::QuadPent_SoundTool::DisableBeatDetect(QP::SoundChannel& ChannelVar) {
	ChannelVar.Channel->removeDSP(BeatDetector);
}

void QP::QuadPent_SoundTool::DisableFreqCutOff(QP::SoundChannel& ChannelVar) {
	ChannelVar.Channel->removeDSP(LowPass);
}

void QP::QuadPent_SoundTool::StopAllSounds() {
	FMOD::ChannelGroup* MasterChannelGroup{};

	SoundSystem->getMasterChannelGroup(&MasterChannelGroup);
	Result = MasterChannelGroup->stop();
}

void QP::QuadPent_SoundTool::SetDistance(QP::SoundChannel& ChannelVar, float MinDist, float MaxDist) {
	ChannelVar.Channel->set3DMinMaxDistance(MinDist, MaxDist);
}

void QP::QuadPent_SoundTool::SetListnerPosition(float X, float Y) {
	ListenerPosition.x = X;
	ListenerPosition.y = Y;
	ListenerPosition.z = 0.0f;

	SoundSystem->set3DListenerAttributes(0, &ListenerPosition, 0, 0, 0);
};

void QP::QuadPent_SoundTool::SetListnerPosition(QP::Vector2& Position) {
	ListenerPosition.x = Position.x;
	ListenerPosition.y = Position.y;
	ListenerPosition.z = 0.0f;

	SoundSystem->set3DListenerAttributes(0, &ListenerPosition, 0, 0, 0);
}

void QP::QuadPent_SoundTool::SetSoundPosition(QP::SoundChannel& ChannelVar, float X, float Y, float Diff) {
	SoundPosition.x = X;
	SoundPosition.y = Y;
	SoundPosition.z = Diff;

	ChannelVar.Channel->set3DAttributes(&SoundPosition, 0);
}

void QP::QuadPent_SoundTool::SetSoundPosition(QP::SoundChannel& ChannelVar, QP::Vector2& Position, float Diff) {
	SoundPosition.x = Position.x;
	SoundPosition.y = Position.y;
	SoundPosition.z = Diff;

	ChannelVar.Channel->set3DAttributes(&SoundPosition, 0);
}