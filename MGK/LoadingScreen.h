#pragma once
#include "Scene.h"
#include "ImageUtil.h"
#include "SoundUtil.h"
#include "IntroMode.h"
#include "CameraUtil.h"
#include "FontUtil.h"

DWORD WINAPI SystemResourceCreateThread(LPVOID Param) {
	soundUtil.Import(IntroSound, MGK_LOGO_SOUND_DIRECTORY, FMOD_DEFAULT);
	imageUtil.PreLoad(MGK_LOGO, MGK_LOGO_DIRECTORY, IMAGE_TYPE_LINEAR);
	imageUtil.PreLoad(FMOD_LOGO, FMOD_LOGO_DIRECTORY, IMAGE_TYPE_LINEAR);
	imageUtil.PreLoad(COLOR_TEXTURE, LINE_TEXTURE_DIRECTORY);

	GLU_CIRCLE = gluNewQuadric();
	GLU_LINE_CIRCLE = gluNewQuadric();
	gluQuadricDrawStyle(GLU_CIRCLE, GLU_FILL);
	gluQuadricDrawStyle(GLU_LINE_CIRCLE, GLU_FILL);
	
#ifdef USE_CUSTOM_FONT
	int TotalSize = sizeof(FONT_PATH);
	int ElementSize = sizeof(FONT_PATH[0]);
	int Length = TotalSize / ElementSize;
	for (int i = 0; i < Length; ++i) {
		FontUtil::Import(FONT_PATH[i]);
	}
#endif

	return 0;
}


class LoadingScreen : public GameObject {
private:
	HANDLE ThreadHandle{};
	GLfloat Rotation{};
	GLfloat Transparent{1.0};
	bool LoadCommand{};

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

	void UpdateFunc(float FT) {
		if (LoadCommand) {
			Rotation -= 200 * FT;

			if (!ThreadUtil::IsRunning(ThreadHandle)) {
				ThreadUtil::Delete(ThreadHandle);
				imageUtil.FinishLoad();

				if (!ENABLE_INTRO_SCREEN) {
					soundUtil.Release(IntroSound);
					scene.SwitchMode(START_MODE);
				}

				else {
					Transparent -= FT * 2.0;
					EX::ClampValue(Transparent, 0.0, CLAMP_LESS);
					if(Transparent == 0.0)
						scene.SwitchMode(IntroMode::Start);
				}
			}
		}

		else {
			camera.Init();
			imageUtil.Init();
			soundUtil.Init();

			imageUtil.Import(ImageSpinner, MGK_LOADING_SPINNER_DIRECTORY, IMAGE_TYPE_LINEAR);
			ThreadUtil::New(ThreadHandle, SystemResourceCreateThread);

			LoadCommand = true;
		}
	}

	void RenderFunc() {
		RenderImage(RENDER_TYPE_STATIC, ImageSpinner, ASP(1.0) - 0.15, -1.0 + 0.15, 0.25, 0.25, Rotation, Transparent);
	}
};

