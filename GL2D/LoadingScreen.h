#pragma once
#include "Scene.h"
#include "ImageUtil.h"
#include "SoundUtil.h"
#include "IntroMode.h"
#include "CameraUtil.h"

class LoadingScreen : public GameObject {
private:
	TextUtil text;
	
public:
	void InputKey(int State, unsigned char NormalKey, int SpecialKey) {
		if (State == NORMAL_KEY_DOWN) {
			switch (NormalKey) {
			case NK_ESCAPE:
				scene.Exit();
				break;
			}
		}
	}

	LoadingScreen() {
		camera.Init();

		SetBackColor(0.0, 0.0, 0.0);
		text.Init(L"Arial", FW_BOLD);
		text.SetColor(1.0, 1.0, 1.0);
	}

	void UpdateFunc(float FT) {
		text.Render(0.5, -0.9, 0.2, 1.0, L"LOADING...");

		imageUtil.Init();
		soundUtil.Init();

		soundUtil.Import(IntroSound, "SystemResource//Sound//MGK_Logo_sound.wav", FMOD_DEFAULT);
		imageUtil.Import(ImageEngineLogo, "SystemResource//Image//Common//MGK_Logo.png");
		imageUtil.Import(ImageFMODLogo, "SystemResource//Image//Common//FMOD_Logo.png");

		imageUtil.Import(ImageCollisionSphere, "SystemResource//Image//Collision//Circle.png", IMAGE_TYPE_NEAREST);
		imageUtil.Import(ImageCollidedSphere, "SystemResource//Image//Collision//Circle Inside.png", IMAGE_TYPE_NEAREST);
		imageUtil.Import(LineTex, "SystemResource//Image//Common//LineTex.png", IMAGE_TYPE_NEAREST);

		if (!ENABLE_INTRO_SCREEN) {
			SetBackColor(0.5, 0.5, 0.5);
			soundUtil.Release(IntroSound);
			imageUtil.Release(ImageEngineLogo);
			imageUtil.Release(ImageFMODLogo);

			scene.SwitchMode(StartMode);
		}

		else {
			scene.SwitchMode(IntroMode::Start);
		}
	}
};

