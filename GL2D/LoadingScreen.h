#pragma once
#include "Framework.h"
#include "ImageUtil.h"
#include "SoundUtil.h"
#include "IntroMode.h"
#include "CameraUtil.h"

class LoadingScreen : public GameObject {
private:
	TextUtil text;
	
public:
	LoadingScreen() {
		camera.Init();

		SetBackColor(0.0, 0.0, 0.0);
		text.Init(L"Arial", FW_BOLD);
		text.SetColor(1.0, 1.0, 1.0);
		text.SetRenderType(RenderType::Static);
	}

	void Update(float FT) {
		text.Render(0.5, -0.9, 0.2, 1.0, L"LOADING...");

		imageUtil.Init();
		soundUtil.Init();

		soundUtil.Import(IntroSound, "SystemResource//Sound//mata_ENGINE Logo Sound.wav", FMOD_DEFAULT);
		imageUtil.Import(ImageEngineLogo, "SystemResource//Image//Common//mata_ENGINE Logo.png", ImageType::Linear);
		imageUtil.Import(ImageFMODLogo, "SystemResource//Image//Common//FMOD Logo.png", ImageType::Linear);

		imageUtil.Import(ImageCollisionBox, "SystemResource//Image//Collision//Square.png", ImageType::Nearest);
		imageUtil.Import(ImageCollidedBox, "SystemResource//Image//Collision//Square Inside.png", ImageType::Nearest);
		imageUtil.Import(ImageCollisionSphere, "SystemResource//Image//Collision//Circle.png", ImageType::Nearest);
		imageUtil.Import(ImageCollidedSphere, "SystemResource//Image//Collision//Circle Inside.png", ImageType::Nearest);

		if (!ENABLE_INTRO_SCREEN) {
			SetBackColor(0.5, 0.5, 0.5);
			framework.SwitchMode(StartMode);
		}

		else
			framework.SwitchMode(IntroMode::Start);
	}
};

