#include "SystemResources.h"
#include "ImageUtil.h"
#include "SoundUtil.h"

Image ImageCollisionBox, ImageCollidedBox, ImageCollisionSphere, ImageCollidedSphere;
Image ImageEngineLogo, ImageFMODLogo;
Sound IntroSound;

void LoadSystemResources() {
	imageUtil.ImportImage(ImageCollisionBox, "SystemResource//Image//Collision//Square.png", ImageType::Nearest);
	imageUtil.ImportImage(ImageCollidedBox, "SystemResource//Image//Collision//Square Inside.png", ImageType::Nearest);
	imageUtil.ImportImage(ImageCollisionSphere, "SystemResource//Image//Collision//Circle.png", ImageType::Nearest);
	imageUtil.ImportImage(ImageCollidedSphere, "SystemResource//Image//Collision//Circle Inside.png", ImageType::Nearest);

	imageUtil.ImportImage(ImageEngineLogo, "SystemResource//Image//Common//mata_ENGINE Logo.png", ImageType::Linear);
	imageUtil.ImportImage(ImageFMODLogo, "SystemResource//Image//Common//FMOD Logo.png", ImageType::Linear);

	soundUtil.ImportSound(IntroSound, "SystemResource//Sound//mata_ENGINE Logo Sound.wav", FMOD_DEFAULT);
}