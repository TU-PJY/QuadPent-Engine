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
	HANDLE  SystemResourceLoadHandle{};
	bool    LoadCommand{};
	bool    SystemResourceLoadEnd{};

	HANDLE  ImageResourceLoadHandle{};
	HANDLE  SoundResourceLoadHandle{};
	HANDLE  FileResourceLoadHandle{};
	HANDLE  FontResourceLoadHandle{};

	bool    ImageResourceLoadEnd{};
	bool    SoundResourceLoadEnd{};
	bool    FileResourceLoadEnd{};
	bool    FontResourceLoadEnd{};

	GLfloat Rotation{};
	GLfloat SpinnerOpacity{ 1.0 };

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
		if (!LoadCommand) {
			camera.Init();
			imageUtil.Init();
			soundUtil.Init();

			imageUtil.Load(SysRes.LOADING_SPINNER, SysRes.SDK_LOADING_SPINNER_DIRECTORY, IMAGE_TYPE_LINEAR);
			threadUtil.Create(SystemResourceLoadHandle, SystemResourceLoader);

			LoadCommand = true;
		}

		else {
			Rotation -= 200 * FrameTime;

			if (LoadResources()) {
				imageUtil.Map();

				if (!ENABLE_INTRO_SCREEN) {
					if (SHOW_FPS)  AddFPSIndicator();
					scene.SwitchMode(START_MODE);
				}

				else {
					SpinnerOpacity -= FrameTime * 2.0;
					if (EX.CheckClampValue(SpinnerOpacity, 0.0, CLAMP_LESS)) {
						if (SHOW_FPS)  AddFPSIndicator();
						scene.SwitchMode(IntroMode.Start);
					}
				}
			}
		}
	}

	void RenderFunc() {
		Begin(RENDER_TYPE_STATIC);
		transform.Move(MoveMatrix, WindowRect.rx - 0.15, -0.85);
		transform.Scale(ScaleMatrix, 0.25, 0.25);
		transform.Rotate(RotateMatrix, Rotation);
		RenderImg(SysRes.LOADING_SPINNER, SpinnerOpacity);
	}

	void AddFPSIndicator() {
		scene.AddObject(new FPS_Indicator, "SDK_OBJECT_FPS_INDICATOR", EOL - 1, OBJECT_TYPE_STATIC);
		Indicator = scene.Find("SDK_OBJECT_FPS_INDICATOR");
	}

	bool LoadResources() {
		if (!threadUtil.IsRunning(SystemResourceLoadHandle) && !SystemResourceLoadEnd) {
			threadUtil.Close(SystemResourceLoadHandle);
			threadUtil.Create(ImageResourceLoadHandle, ImageResourceLoader);
			threadUtil.Create(SoundResourceLoadHandle, SoundResourceLoader);
			threadUtil.Create(FileResourceLoadHandle, DataResourceLoader);
			threadUtil.Create(FontResourceLoadHandle, FontResourceLoader);
			std::cout << "System resource load completed." << std::endl;
			SystemResourceLoadEnd = true;
		}

		if (!ImageResourceLoadEnd && !threadUtil.IsRunning(ImageResourceLoadHandle)) {
			threadUtil.Close(ImageResourceLoadHandle);
			std::cout << "Image resource load completed." << std::endl;
			ImageResourceLoadEnd = true;
		}

		if (!SoundResourceLoadEnd && !threadUtil.IsRunning(SoundResourceLoadHandle)) {
			threadUtil.Close(SoundResourceLoadHandle);
			std::cout << "Sound resource load completed." << std::endl;
			SoundResourceLoadEnd = true;
		}

		if (!FileResourceLoadEnd && !threadUtil.IsRunning(FileResourceLoadHandle)) {
			threadUtil.Close(FileResourceLoadHandle);
			std::cout << "File resource load completed." << std::endl;
			FileResourceLoadEnd = true;
		}

		if (!FontResourceLoadEnd && !threadUtil.IsRunning(FontResourceLoadHandle)) {
			threadUtil.Close(FontResourceLoadHandle);
			std::cout << "Font resource load completed." << std::endl;
			FontResourceLoadEnd = true;
		}

		if (SystemResourceLoadEnd && ImageResourceLoadEnd && SoundResourceLoadEnd && FileResourceLoadEnd && FontResourceLoadEnd)
			return true;

		return false;
	}

	static DWORD WINAPI SystemResourceLoader(LPVOID Param) {
		soundUtil.Load(SysRes.INTRO_SOUND, SysRes.SDK_LOGO_SOUND_DIRECTORY, FMOD_DEFAULT);
		imageUtil.LoadT(SysRes.SDK_LOGO, SysRes.SDK_LOGO_IMAGE_DIRECTORY, IMAGE_TYPE_LINEAR);
		imageUtil.LoadT(SysRes.SDK_LOGO_ERROR, SysRes.SDK_LOGO_ERROR_IMAGE_DIRECTORY, IMAGE_TYPE_LINEAR);
		imageUtil.LoadT(SysRes.FMOD_LOGO, SysRes.FMOD_LOGO_DIRECTORY, IMAGE_TYPE_LINEAR);
		imageUtil.LoadT(SysRes.COLOR_TEXTURE, SysRes.COLOR_TEXTURE_DIRECTORY);

		SysRes.GLU_CIRCLE = gluNewQuadric();
		SysRes.GLU_LINE_CIRCLE = gluNewQuadric();
		gluQuadricDrawStyle(SysRes.GLU_CIRCLE, GLU_FILL);
		gluQuadricDrawStyle(SysRes.GLU_LINE_CIRCLE, GLU_FILL);

		fontUtil.LoadT(SysRes.SYSTEM_FONT_DIRECTORY);

		return 0;
	}
};

