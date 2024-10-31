#pragma once
#include "Scene.h"
#include "ImageUtil.h"
#include "SoundUtil.h"
#include "IntroMode.h"
#include "CameraUtil.h"
#include "FontUtil.h"
#include "GameResource.h"

DWORD WINAPI SystemFileLoadThread(LPVOID Param) {
	soundUtil.Import(IntroSound, "MGKResource//Sound//MGK_Logo_Sound.wav", FMOD_DEFAULT);
	imageUtil.PreLoad(ImageEngineLogo, "MGKResource//Image//Common//MGK_Logo.png", IMAGE_TYPE_LINEAR);
	imageUtil.PreLoad(ImageFMODLogo, "MGKResource//Image//Common//FMOD_Logo.png", IMAGE_TYPE_LINEAR);
	imageUtil.PreLoad(ImageCollisionSphere, "MGKResource//Image//Collision//Circle.png");
	imageUtil.PreLoad(ImageCollidedSphere, "MGKResource//Image//Collision//Circle_Inside.png");
	imageUtil.PreLoad(LineTex, "MGKResource//Image//ShapeUtil//Texture_Line.png");
	
#ifdef USE_CUSTOM_FONT
	int TotalSize = sizeof(FONT_PATH);
	int ElementSize = sizeof(FONT_PATH[0]);
	int Length = TotalSize / ElementSize;
	for (int i = 0; i < Length; ++i) {
		DWORD Num = i;
		FontUtil::Import(FONT_PATH[i], Num);
	}
#endif

	return 0;
}


class LoadingScreen : public GameObject {
private:
	HANDLE ThreadHandle{};
	GLfloat Rotation{};

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
		SetColor(1.0, 1.0, 1.0);

		camera.Init();
		imageUtil.Init();
		soundUtil.Init();

		imageUtil.Import(ImageSpinner, "MGKResource//Image//Common//MGK_Loading_Spinner.png", IMAGE_TYPE_LINEAR);

		ThreadUtil::New(ThreadHandle, SystemFileLoadThread);
	}

	void UpdateFunc(float FT) {
		Rotation -= 200 * FT;

		if (!ThreadUtil::GetState(ThreadHandle)) {
			ThreadUtil::Delete(ThreadHandle);
			imageUtil.FinishLoad();
			
			if (!ENABLE_INTRO_SCREEN) {
				soundUtil.Release(IntroSound);
				imageUtil.Release(ImageEngineLogo);
				imageUtil.Release(ImageFMODLogo);

				scene.SwitchMode(START_MODE);
			}

			else
				scene.SwitchMode(IntroMode::Start);
		}
	}

	void RenderFunc() {
		InitMatrix(RENDER_TYPE_STATIC);
		Transform::Rotate(RotateMatrix, Rotation);
		Render(ImageSpinner);
	}
};

