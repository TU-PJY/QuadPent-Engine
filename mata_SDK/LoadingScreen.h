#pragma once
#include "Scene.h"
#include "ImageUtil.h"
#include "SoundUtil.h"
#include "IntroMode.h"
#include "CameraUtil.h"
#include "FontUtil.h"
#include "FPSInd.h"

class LoadingScreen : public GameObject {
private:
	HANDLE  ThreadHandle{};
	GLfloat Rotation{};
	GLfloat SpinnerOpacity{1.0};
	bool    LoadCommand{};
	bool    ThreadEnd{};

public:
	void InputKey(KeyEvent& Event) {
#ifdef ENABLE_DEV_EXIT
		if (Event.Type == NORMAL_KEY_DOWN) {
			if (Event.NormalKey == NK_ESCAPE)
				System.Exit();
		}
#endif
	}

	void UpdateFunc(float FrameTime) {
		if (LoadCommand) {
			Rotation -= 200 * FrameTime;

			if (!threadUtil.IsRunning(ThreadHandle) && !ThreadEnd) {
				threadUtil.Close(ThreadHandle);
				ThreadEnd = true;
			}

			if(ThreadEnd) {
				imageUtil.Map();

				if (!ENABLE_INTRO_SCREEN) {
#ifdef USE_SOUND_SYSTEM
					soundUtil.Release(SysRes.INTRO_SOUND);
#endif				
					if (SHOW_FPS) {
						scene.AddObject(new FPS_Indicator, "MGK_OBJECT_FPS_INDICATOR", EOL - 1, OBJECT_TYPE_STATIC);
						Indicator = scene.Find("MGK_OBJECT_FPS_INDICATOR");
					}
					scene.SwitchMode(START_MODE);
				}

				else {
					SpinnerOpacity -= FrameTime * 2.0;
					if (EX.CheckClampValue(SpinnerOpacity, 0.0, CLAMP_LESS)) {
						if (SHOW_FPS) {
							scene.AddObject(new FPS_Indicator, "MGK_OBJECT_FPS_INDICATOR", EOL - 1, OBJECT_TYPE_STATIC);
							Indicator = scene.Find("MGK_OBJECT_FPS_INDICATOR");
						}
						scene.SwitchMode(IntroMode.Start);
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
			imageUtil.Load(SysRes.LOADING_SPINNER, SysRes.SDK_LOADING_SPINNER_DIRECTORY, IMAGE_TYPE_LINEAR);
			threadUtil.Create(ThreadHandle, SystemResourceCreateThread);

			LoadCommand = true;
		}
	}

	void RenderFunc() {
		BeginRender(RENDER_TYPE_STATIC);
		transform.Move(TranslateMatrix, WindowRect.rx - 0.15, -0.85);
		transform.Scale(ScaleMatrix, 0.25, 0.25);
		transform.Rotate(RotateMatrix, Rotation);
		RenderSprite(SysRes.LOADING_SPINNER, SpinnerOpacity);
	}

	static DWORD WINAPI SystemResourceCreateThread(LPVOID Param) {
#ifdef USE_SOUND_SYSTEM
		soundUtil.Load(SysRes.INTRO_SOUND, SysRes.SDK_LOGO_SOUND_DIRECTORY, FMOD_DEFAULT);
#endif

		imageUtil.PreLoad(SysRes.SDK_LOGO, SysRes.SDK_LOGO_IMAGE_DIRECTORY, IMAGE_TYPE_LINEAR);
		imageUtil.PreLoad(SysRes.FMOD_LOGO, SysRes.FMOD_LOGO_DIRECTORY, IMAGE_TYPE_LINEAR);
		imageUtil.PreLoad(SysRes.COLOR_TEXTURE, SysRes.COLOR_TEXTURE_DIRECTORY);

		SysRes.GLU_CIRCLE = gluNewQuadric();
		SysRes.GLU_LINE_CIRCLE = gluNewQuadric();
		gluQuadricDrawStyle(SysRes.GLU_CIRCLE, GLU_FILL);
		gluQuadricDrawStyle(SysRes.GLU_LINE_CIRCLE, GLU_FILL);

		fontUtil.Load(SysRes.SYSTEM_FONT_DIRECTORY, true);

#ifdef USE_CUSTOM_FONT
		int TotalSize = sizeof(FONT_PATH);
		int ElementSize = sizeof(FONT_PATH[0]);
		int Length = TotalSize / ElementSize;
		for (int i = 0; i < Length; ++i)
			fontUtil.Load(FONT_PATH[i], true);
#endif

		return 0;
	}
};

