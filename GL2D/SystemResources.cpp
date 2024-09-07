#include "EngineHeader.h"

std::vector<ImageInfo> SystemImageList
{
	{"BOUNDBOX", "SystemResource//Image//Collision//Square.png"},
	{"BOUNDBOXCOLLISION", "SystemResource//Image//Collision//Square Inside.png"},
	{"CIRCLE", "SystemResource//Image////Collision//Circle.png"},
	{"CIRCLECOLLISION", "SystemResource//Image////Collision//Circle Inside.png"},
	{"FMOD_LOGO", "SystemResource//Image//Common//FMOD Logo.png"},
	{"mata_ENGINE_LOGO", "SystemResource//Image//Common//mata_ENGINE Logo.png"},
};

std::vector<FileNameAndOption> SystemSoundList
{
	{"mata_ENGINE_LOGO_SOUND", "SystemResource//Sound//mata_ENGINE Logo Sound.wav", FMOD_DEFAULT},
	{"mata_ENGINE_ERROR_SOUND", "SystemResource//Sound//mata_ENGINE crash.wav", FMOD_DEFAULT}
};