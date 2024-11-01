#pragma once
#include "Scene.h"
#include "ImageUtil.h"
#include "SoundUtil.h"
#include "IntroMode.h"
#include "CameraUtil.h"
#include "FontUtil.h"

DWORD WINAPI SystemFileLoadThread(LPVOID Param) {
	soundUtil.Import(IntroSound, MGK_LOGO_SOUND_DIRECTORY, FMOD_DEFAULT);
	imageUtil.PreLoad(ImageEngineLogo, MGK_LOGO_DIRECTORY, IMAGE_TYPE_LINEAR);
	imageUtil.PreLoad(ImageFMODLogo, FMOD_LOGO_DIRECTORY, IMAGE_TYPE_LINEAR);
	imageUtil.PreLoad(ImageCollisionSphere, COLLISION_SPHERE_DIRECTORY);
	imageUtil.PreLoad(ImageCollidedSphere, COLLIDED_SPHERE_DIRECTORY);
	imageUtil.PreLoad(LineTex, LINE_TEXTURE_DIRECTORY);
	
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
				Framework::Exit();
				break;
			}
		}
	}

	LoadingScreen() {
		SetColor(1.0, 1.0, 1.0);

		camera.Init();
		imageUtil.Init();
		soundUtil.Init();

		imageUtil.Import(ImageSpinner, MGK_LOADING_SPINNER_DIRECTORY, IMAGE_TYPE_LINEAR);

		ThreadUtil::New(ThreadHandle, SystemFileLoadThread);
	}

	void UpdateFunc(float FT) {
		Rotation -= 200 * FT;

		if (!ThreadUtil::GetState(ThreadHandle)) {
			ThreadUtil::Delete(ThreadHandle);
			imageUtil.FinishLoad();
			
			if (!ENABLE_INTRO_SCREEN) {
				soundUtil.Release(IntroSound);
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

