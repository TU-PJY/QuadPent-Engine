#include "SoundUtil.h"

struct FileNameAndOption {
	const char* FileName;
	FMOD_MODE Option;
};
FileNameAndOption FNO;


// list sounds to load
std::unordered_map<std::string, FileNameAndOption> SoundList
{
	
};

// list channels to load
std::vector<std::string> ChannelList
{
	
};


void SoundUtil::Init() {
	f_result = FMOD::System_Create(&SoundSystem);

	if (f_result != FMOD_OK)	
		exit(EXIT_FAILURE);

	SoundSystem->init(32, FMOD_INIT_NORMAL, extdvdata);

	LoadSoundFromList();
	LoadChannelFromList();
}

void SoundUtil::LoadSoundFromList() {
	for (auto& [SoundName, Struct] : SoundList) {
		SoundSystem->createSound(Struct.FileName, Struct.Option, 0, &FSound);
		LoadedSoundList.insert(std::pair(SoundName, FSound));
	}
	FSound = nullptr;
}

void SoundUtil::LoadChannelFromList() {
	for (auto& s : ChannelList) {
		FMOD::Channel* channel;
		LoadedChannelList.insert(std::pair(s, channel));
	}
}

void SoundUtil::PlaySound(std::string SoundName, std::string ChannelName) {
	SoundSystem->playSound(LoadedSoundList.find(SoundName)->second, 0, false, &LoadedChannelList.find(ChannelName)->second);
}

void SoundUtil::StopSound(std::string ChannelName) {
	LoadedChannelList.find(ChannelName)->second->stop();
}