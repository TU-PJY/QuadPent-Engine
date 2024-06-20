#pragma once
#include "fmod.hpp"
#include "fmod_errors.h"
#include <unordered_map>
#include <string>

class SoundUtil {
private:
	std::unordered_map<std::string, FMOD::Sound*> LoadedSoundList;
	std::unordered_map<std::string, FMOD::Channel*> LoadedChannelList;
	FMOD::System* SoundSystem;
	FMOD::Sound* FSound;

	FMOD_RESULT f_result;
	void* extdvdata;

public:
	void Init();
	void LoadSoundFromList();
	void LoadChannelFromList();
	void PlaySound(std::string SoundName, std::string ChannelName);
	void StopSound(std::string ChannelName);
};
extern SoundUtil soundUtil;
