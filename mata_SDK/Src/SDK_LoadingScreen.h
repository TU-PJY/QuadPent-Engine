#pragma once
#include "SDK_Scene.h"
#include "SDK_ImageTool.h"
#include "SDK_SoundTool.h"
#include "SDK_Camera.h"
#include "SDK_FontLoader.h"
#include "SDK_IntroMode.h"
#include "SDK_FPSIndicator.h"

class SDK_LoadingScreen : public MSDK::Object {
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

	float Rotation{};
	float SpinnerOpacity{ 1.0 };


public:
	void UpdateFunc(float FrameTime) {
		if (!LoadStart) {
			MSDK::Camera.Init();
			MSDK::ImageTool.Init();
			MSDK::SoundTool.Init();

			MSDK::FontLoader.LoadT(MSDK::SYSRES.SDK_FONT_DIRECTORY);
			MSDK::ImageTool.LoadImage(MSDK::SYSRES.SDK_IMAGE_ERROR, MSDK::SYSRES.SDK_ERROR_IMAGE_DIRECTORY, IMAGE_TYPE_LINEAR);
			MSDK::ImageTool.LoadImage(MSDK::SYSRES.LOADING_SPINNER, MSDK::SYSRES.SDK_LOADING_SPINNER_DIRECTORY, IMAGE_TYPE_LINEAR);
			MSDK::ImageTool.LoadImage(MSDK::SYSRES.MATA_LOGO, MSDK::SYSRES.MATA_LOGO_IMAGE_DIRECTORY, IMAGE_TYPE_LINEAR);

			MSDK::ThreadTool.Create(SystemResourceLoadHandle, SystemResourceLoader);

			LoadStart = true;
		}

		else {
			Rotation += 200 * FrameTime;

			if (LoadResources()) {
				if (!InitializationEnd) {
					MSDK::ImageTool.Map();
					std::cout << "All of Image resources mapped." << std::endl;

					ModeAttribute();
					std::cout << "All of Mode pointers mapped." << std::endl;

					InitializationEnd = true;
				}

				if (!ENABLE_INTRO_SCREEN) {
					if (InitializationEnd) {
						if (SHOW_FPS)  AddFPSIndicator();
						MSDK::Scene.SwitchMode(MSDK::START_MODE);
					}
				}

				else {
					if (InitializationEnd) {
						SpinnerOpacity -= FrameTime * 2.0;
						if (MSDK::EXTool.CheckClampValue(SpinnerOpacity, 0.0, CLAMP_LESS)) {
							if (SHOW_FPS)  AddFPSIndicator();
							MSDK::Scene.SwitchMode(IntroMode.Start);
						}
					}
				}
			}
		}
	}

	void RenderFunc() {
		Begin(RENDER_TYPE_STATIC);
		MSDK::Transform.Scale(MSDK::MoveMatrix, 0.6, 0.6);
		MSDK::ImageTool.RenderImage(MSDK::SYSRES.MATA_LOGO, SpinnerOpacity);

		Begin(RENDER_TYPE_STATIC);
		MSDK::Transform.Scale(MSDK::MoveMatrix, 1.0, 1.0);
		MSDK::Transform.Rotate(MSDK::MoveMatrix, Rotation);
		MSDK::ImageTool.RenderImage(MSDK::SYSRES.LOADING_SPINNER, SpinnerOpacity);
	}

	void AddFPSIndicator() {
		Indicator = MSDK::Scene.AddObject(new SDK_FPS_Indicator, "SDK_OBJECT_FPS_INDICATOR", EOL - 1, OBJECT_TYPE_STATIC);
	}

	bool LoadResources() {
		if (!MSDK::ThreadTool.CheckAlive(SystemResourceLoadHandle) && !SystemResourceLoadEnd) {
			MSDK::ThreadTool.Close(SystemResourceLoadHandle);
			MSDK::ThreadTool.Create(ImageResourceLoadHandle, ImageResourceLoader);
			MSDK::ThreadTool.Create(SoundResourceLoadHandle, SoundResourceLoader);
			MSDK::ThreadTool.Create(DataResourceLoadHandle, FileResourceLoader);
			MSDK::ThreadTool.Create(FontResourceLoadHandle, FontResourceLoader);
			std::cout << "System resource load completed." << std::endl;

			SystemResourceLoadEnd = true;
		}

		if (!ImageResourceLoadEnd && !MSDK::ThreadTool.CheckAlive(ImageResourceLoadHandle)) {
			MSDK::ThreadTool.Close(ImageResourceLoadHandle);
			std::cout << "Image resource load completed." << std::endl;
			ImageResourceLoadEnd = true;
		}

		if (!SoundResourceLoadEnd && !MSDK::ThreadTool.CheckAlive(SoundResourceLoadHandle)) {
			MSDK::ThreadTool.Close(SoundResourceLoadHandle);
			std::cout << "Sound resource load completed." << std::endl;
			SoundResourceLoadEnd = true;
		}

		if (!DataResourceLoadEnd && !MSDK::ThreadTool.CheckAlive(DataResourceLoadHandle)) {
			MSDK::ThreadTool.Close(DataResourceLoadHandle);
			std::cout << "Data resource load completed." << std::endl;
			DataResourceLoadEnd = true;
		}

		if (!FontResourceLoadEnd && !MSDK::ThreadTool.CheckAlive(FontResourceLoadHandle)) {
			MSDK::ThreadTool.Close(FontResourceLoadHandle);
			std::cout << "Font resource load completed." << std::endl;
			FontResourceLoadEnd = true;
		}

		if (SystemResourceLoadEnd && ImageResourceLoadEnd && SoundResourceLoadEnd && DataResourceLoadEnd && FontResourceLoadEnd)
			return true;

		return false;
	}

	static DWORD WINAPI SystemResourceLoader(LPVOID Param) {
		MSDK::ImageTool.LoadImageT(MSDK::SYSRES.SDK_LOGO, MSDK::SYSRES.SDK_LOGO_IMAGE_DIRECTORY, IMAGE_TYPE_LINEAR);
		MSDK::ImageTool.LoadImageT(MSDK::SYSRES.FMOD_LOGO, MSDK::SYSRES.FMOD_LOGO_DIRECTORY, IMAGE_TYPE_LINEAR);
		MSDK::ImageTool.LoadImageT(MSDK::SYSRES.COLOR_TEXTURE, MSDK::SYSRES.COLOR_TEXTURE_DIRECTORY);
		MSDK::SoundTool.Load(MSDK::SYSRES.INTRO_SOUND, MSDK::SYSRES.SDK_LOGO_SOUND_DIRECTORY);

		MSDK::SYSRES.GLU_CIRCLE = gluNewQuadric();
		MSDK::SYSRES.GLU_LINE_CIRCLE = gluNewQuadric();
		gluQuadricDrawStyle(MSDK::SYSRES.GLU_CIRCLE, GLU_FILL);
		gluQuadricDrawStyle(MSDK::SYSRES.GLU_LINE_CIRCLE, GLU_FILL);

		return 0;
	}
};

