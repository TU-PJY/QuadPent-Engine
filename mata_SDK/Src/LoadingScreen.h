#pragma once
#include "Scene.h"
#include "ImageUtil.h"
#include "SoundUtil.h"
#include "IntroMode.h"
#include "CameraUtil.h"
#include "FontUtil.h"
#include "FPSInd.h"

class LoadingScreen : public Object {
private:
	HANDLE  SystemResourceLoadHandle{};
	bool    LoadStart{};
	bool    SystemResourceLoadEnd{};

	HANDLE  ImageResourceLoadHandle{};
	HANDLE  SoundResourceLoadHandle{};
	HANDLE  DataResourceLoadHandle{};
	HANDLE  FontResourceLoadHandle{};

	bool    ImageResourceLoadEnd{};
	bool    SoundResourceLoadEnd{};
	bool    DataResourceLoadEnd{};
	bool    FontResourceLoadEnd{};
	bool    InitializationEnd{};

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
		if (!LoadStart) {
			camera.Init();
			imageUtil.Init();
			soundUtil.Init();

			fontUtil.LoadT(SYSRES.SDK_FONT_DIRECTORY);
			imageUtil.LoadImage(SYSRES.SDK_IMAGE_ERROR, SYSRES.SDK_ERROR_IMAGE_DIRECTORY, IMAGE_TYPE_LINEAR);
			imageUtil.LoadImage(SYSRES.LOADING_SPINNER, SYSRES.SDK_LOADING_SPINNER_DIRECTORY, IMAGE_TYPE_LINEAR);

			threadUtil.Create(SystemResourceLoadHandle, SystemResourceLoader);

			LoadStart = true;
		}

		else {
			Rotation += 200 * FrameTime;

			if (LoadResources()) {
				if (!InitializationEnd) {
					imageUtil.Map();
					std::cout << "All of Image resources mapped." << std::endl;

					ModeAttribute();
					std::cout << "All of Mode pointers mapped." << std::endl;

					InitializationEnd = true;
				}

				if (!ENABLE_INTRO_SCREEN) {
					if (InitializationEnd) {
						if (SHOW_FPS)  AddFPSIndicator();
						scene.SwitchMode(START_MODE);
					}
				}

				else {
					if (InitializationEnd) {
						SpinnerOpacity -= FrameTime * 2.0;
						if (EX.CheckClampValue(SpinnerOpacity, 0.0, CLAMP_LESS)) {
							if (SHOW_FPS)  AddFPSIndicator();
							scene.SwitchMode(IntroMode.Start);
						}
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
		imageUtil.RenderImage(SYSRES.LOADING_SPINNER, SpinnerOpacity);
	}

	void AddFPSIndicator() {
		scene.AddObject(new FPS_Indicator, "SDK_OBJECT_FPS_INDICATOR", EOL - 1, OBJECT_TYPE_STATIC);
		Indicator = scene.Find("SDK_OBJECT_FPS_INDICATOR");
	}

	bool LoadResources() {
		if (!threadUtil.CheckAlive(SystemResourceLoadHandle) && !SystemResourceLoadEnd) {
			threadUtil.Close(SystemResourceLoadHandle);
			threadUtil.Create(ImageResourceLoadHandle, ImageResourceLoader);
			threadUtil.Create(SoundResourceLoadHandle, SoundResourceLoader);
			threadUtil.Create(DataResourceLoadHandle, FileResourceLoader);
			threadUtil.Create(FontResourceLoadHandle, FontResourceLoader);
			std::cout << "System resource load completed." << std::endl;

			SystemResourceLoadEnd = true;
		}

		if (!ImageResourceLoadEnd && !threadUtil.CheckAlive(ImageResourceLoadHandle)) {
			threadUtil.Close(ImageResourceLoadHandle);
			std::cout << "Image resource load completed." << std::endl;
			ImageResourceLoadEnd = true;
		}

		if (!SoundResourceLoadEnd && !threadUtil.CheckAlive(SoundResourceLoadHandle)) {
			threadUtil.Close(SoundResourceLoadHandle);
			std::cout << "Sound resource load completed." << std::endl;
			SoundResourceLoadEnd = true;
		}

		if (!DataResourceLoadEnd && !threadUtil.CheckAlive(DataResourceLoadHandle)) {
			threadUtil.Close(DataResourceLoadHandle);
			std::cout << "Data resource load completed." << std::endl;
			DataResourceLoadEnd = true;
		}

		if (!FontResourceLoadEnd && !threadUtil.CheckAlive(FontResourceLoadHandle)) {
			threadUtil.Close(FontResourceLoadHandle);
			std::cout << "Font resource load completed." << std::endl;
			FontResourceLoadEnd = true;
		}

		if (SystemResourceLoadEnd && ImageResourceLoadEnd && SoundResourceLoadEnd && DataResourceLoadEnd && FontResourceLoadEnd)
			return true;

		return false;
	}

	static DWORD WINAPI SystemResourceLoader(LPVOID Param) {
		imageUtil.LoadImageT(SYSRES.SDK_LOGO, SYSRES.SDK_LOGO_IMAGE_DIRECTORY, IMAGE_TYPE_LINEAR);
		imageUtil.LoadImageT(SYSRES.FMOD_LOGO, SYSRES.FMOD_LOGO_DIRECTORY, IMAGE_TYPE_LINEAR);
		imageUtil.LoadImageT(SYSRES.COLOR_TEXTURE, SYSRES.COLOR_TEXTURE_DIRECTORY);
		soundUtil.Load(SYSRES.INTRO_SOUND, SYSRES.SDK_LOGO_SOUND_DIRECTORY);

		SYSRES.GLU_CIRCLE = gluNewQuadric();
		SYSRES.GLU_LINE_CIRCLE = gluNewQuadric();
		gluQuadricDrawStyle(SYSRES.GLU_CIRCLE, GLU_FILL);
		gluQuadricDrawStyle(SYSRES.GLU_LINE_CIRCLE, GLU_FILL);

		return 0;
	}
};

