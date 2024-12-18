#pragma once
#include "Scene.h"
#include "ImageUtil.h"
#include "SoundUtil.h"
#include "IntroMode.h"
#include "CameraUtil.h"
#include "FontUtil.h"
#include "FPSInd.h"

DWORD WINAPI SystemResourceCreateThread(LPVOID Param) {
#ifdef USE_SOUND_SYSTEM
	soundUtil.Import(INTRO_SOUND, MGK_LOGO_SOUND_DIRECTORY, FMOD_DEFAULT);
#endif

	imageUtil.PreLoad(MGK_LOGO, MGK_LOGO_DIRECTORY, IMAGE_TYPE_LINEAR);
	imageUtil.PreLoad(FMOD_LOGO, FMOD_LOGO_DIRECTORY, IMAGE_TYPE_LINEAR);
	imageUtil.PreLoad(COLOR_TEXTURE, LINE_TEXTURE_DIRECTORY);

	GLU_CIRCLE = gluNewQuadric();
	GLU_LINE_CIRCLE = gluNewQuadric();
	gluQuadricDrawStyle(GLU_CIRCLE, GLU_FILL);
	gluQuadricDrawStyle(GLU_LINE_CIRCLE, GLU_FILL);

	FontUtil::Import(ROBOTO_FONT_DIRECTORY, true);

#ifdef USE_CUSTOM_FONT
	int TotalSize = sizeof(FONT_PATH);
	int ElementSize = sizeof(FONT_PATH[0]);
	int Length = TotalSize / ElementSize;
	for (int i = 0; i < Length; ++i)
		FontUtil::Import(FONT_PATH[i], true);
#endif

	return 0;
}


class LoadingScreen : public GameObject {
private:
	HANDLE  ThreadHandle{};
	GLfloat Rotation{};
	GLfloat Transparent{1.0};
	bool    LoadCommand{};
	bool    ThreadEnd{};

public:
	void InputKey(KeyEvent& Event) {
		if (Event.Type == NORMAL_KEY_DOWN) {
			switch (Event.NormalKey) {
			case NK_ESCAPE:
				Framework::Exit();
				break;
			}
		}
	}

	void UpdateFunc(float FT) {
		if (LoadCommand) {
			Rotation -= 200 * FT;

			if (!ThreadUtil::IsRunning(ThreadHandle) && !ThreadEnd) {
				ThreadUtil::Close(ThreadHandle);
				ThreadEnd = true;
			}

			if(ThreadEnd) {
				imageUtil.FinishLoad();

				if (!ENABLE_INTRO_SCREEN) {
#ifdef USE_SOUND_SYSTEM
					soundUtil.Release(INTRO_SOUND);
#endif				
					if(SHOW_FPS)
						scene.AddObject(new FPSInd, "fps_ind", END - 1, OBJECT_TYPE_STATIC);
					scene.SwitchMode(START_MODE);
				}

				else {
					Transparent -= FT * 2.0;
					if (EX::CheckClampValue(Transparent, 0.0, CLAMP_LESS)) {
						if(SHOW_FPS)
							scene.AddObject(new FPSInd, "fps_ind", END - 1, OBJECT_TYPE_STATIC);
						scene.SwitchMode(IntroMode::Start);
					}
				}
			}
		}

		else {
			camera.Init();
			imageUtil.Init();
#ifdef USE_SOUND_SYSTEM
			soundUtil.Init();
#endif

			imageUtil.Import(LOADING_SPINNER, MGK_LOADING_SPINNER_DIRECTORY, IMAGE_TYPE_LINEAR);
			ThreadUtil::Create(ThreadHandle, SystemResourceCreateThread);

			LoadCommand = true;
		}
	}

	void RenderFunc() {
		DrawImage(RENDER_TYPE_STATIC, LOADING_SPINNER, ASP(1.0) - 0.15, -1.0 + 0.15, 0.25, 0.25, Rotation, Transparent);
	}
};

