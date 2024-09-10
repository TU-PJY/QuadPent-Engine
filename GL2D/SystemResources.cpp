#include "EngineHeader.h"
#include "ImageUtil.h"

std::vector<ImageInfo> SystemImageList
{
	{"FMOD_LOGO", "SystemResource//Image//Common//FMOD Logo.png"},
	{"mata_ENGINE_LOGO", "SystemResource//Image//Common//mata_ENGINE Logo.png"},
};

std::vector<FileNameAndOption> SystemSoundList
{
	{"mata_ENGINE_LOGO_SOUND", "SystemResource//Sound//mata_ENGINE Logo Sound.wav", FMOD_DEFAULT},
	{"mata_ENGINE_ERROR_SOUND", "SystemResource//Sound//mata_ENGINE crash.wav", FMOD_DEFAULT}
};

unsigned int ImageCollisionBox, ImageCollidedBox, ImageCollisionSphere, ImageCollidedSphere;

void LoadCollisionImageResources() {
	imageUtil.LoadImageFromFile(ImageCollisionBox, "SystemResource//Image//Collision//Square.png");
	imageUtil.LoadImageFromFile(ImageCollidedBox, "SystemResource//Image//Collision//Square Inside.png");
	imageUtil.LoadImageFromFile(ImageCollisionSphere, "SystemResource//Image//Collision//Circle.png");
	imageUtil.LoadImageFromFile(ImageCollidedSphere, "SystemResource//Image//Collision//Circle Inside.png");
}