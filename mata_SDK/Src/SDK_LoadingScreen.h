#pragma once
#include "SDK_Scene.h"
#include "SDK_ImageTool.h"
#include "SDK_SoundTool.h"
#include "SDK_Camera.h"
#include "SDK_FontLoader.h"
#include "SDK_IntroMode.h"
#include "SDK_FPSIndicator.h"

class SDK_LoadingScreen : public SDK::Object {
private:
	bool    LoadStart{};

	SDK::ThreadHandle  SystemResourceLoadHandle{};
	SDK::ThreadHandle  ImageResourceLoadHandle{};
	SDK::ThreadHandle  SoundResourceLoadHandle{};
	SDK::ThreadHandle  DataResourceLoadHandle{};
	SDK::ThreadHandle  FontResourceLoadHandle{};

	bool    SystemResourceLoadEnd{};
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
			SDK::Camera.Init();
			SDK::ImageTool.Init();
			SDK::SoundTool.Init();

			SDK::FontLoader.Load(SDK::SYSRES.SDK_FONT_DIRECTORY);
			SDK::SYSRES.SYSTEM_FONT_REGULAR.Init(L"Roboto");
			SDK::SYSRES.SYSTEM_FONT_BOLD.Init(L"Roboto", FW_BOLD);

			SDK::ImageTool.LoadImage(SDK::SYSRES.LOADING_SPINNER, SDK::SYSRES.SDK_LOADING_SPINNER_DIRECTORY, IMAGE_TYPE_LINEAR);
			SDK::ImageTool.LoadImage(SDK::SYSRES.MATA_LOGO, SDK::SYSRES.MATA_LOGO_IMAGE_DIRECTORY, IMAGE_TYPE_LINEAR);
			SDK::ResourcePreLoader();
			std::cout << "Resources pre-loaded." << std::endl;

			SDK::ThreadTool.Create(SystemResourceLoadHandle, SystemResourceLoader);

			LoadStart = true;
		}

		else {
			Rotation += 200 * FrameTime;

			if (LoadResources()) {
				if (!InitializationEnd) {
					SDK::ImageTool.Map();
					std::cout << "All of Image resources mapped." << std::endl;

					ModeAttribute();
					std::cout << "All of Mode pointers mapped." << std::endl;

					InitializationEnd = true;
				}

				else {
					if(ENABLE_INTRO_SCREEN) {
						SpinnerOpacity -= FrameTime * 2.0;
						if (SDK::EXTool.CheckClampValue(SpinnerOpacity, 0.0, CLAMP_LESS)) {
							SDK::Scene.SwitchMode(IntroMode.Start);
							if (SHOW_FPS)  AddFPSIndicator();
							SDK::Scene.DeleteObject(this);
						}
					}

					else {
						SDK::Scene.SwitchMode(SDK::START_MODE);
						if (SHOW_FPS)  AddFPSIndicator();
						SDK::Scene.DeleteObject(this);
						SDK::Scene.LockSystemLayer();
					}
				}
			}
		}
	}

	void RenderFunc() {
		SDK::ResetGlobalAttribute();
		SDK::ImageTool.SetGlobalColor(1.0, 1.0, 1.0);
		{
			SDK::Begin(RENDER_TYPE_STATIC);
			SDK::Transform.Scale(0.6, 0.6);
			SDK::ImageTool.RenderImage(SDK::SYSRES.MATA_LOGO, SpinnerOpacity, true);

			SDK::Begin(RENDER_TYPE_STATIC);
			SDK::Transform.Scale(1.0, 1.0);
			SDK::Transform.Rotate(Rotation);
			SDK::ImageTool.RenderImage(SDK::SYSRES.LOADING_SPINNER, SpinnerOpacity, true);
		} 
		SDK::ResetGlobalAttribute();
	}

	void AddFPSIndicator() {
		SDK::Scene.AddSystemObject(new SDK_FPS_Indicator);
	}

	bool LoadResources() {
		if (!SystemResourceLoadEnd && !SDK::ThreadTool.CheckRunning(SystemResourceLoadHandle)) {
			SDK::ThreadTool.Close(SystemResourceLoadHandle);
			SDK::ThreadTool.Create(ImageResourceLoadHandle, SDK::ImageResourceLoader);
			SDK::ThreadTool.Create(SoundResourceLoadHandle, SDK::SoundResourceLoader);
			SDK::ThreadTool.Create(DataResourceLoadHandle, SDK::FileResourceLoader);
			SDK::ThreadTool.Create(FontResourceLoadHandle, SDK::FontResourceLoader);
			std::cout << "System resource load completed." << std::endl;

			SystemResourceLoadEnd = true;
		}

		if (!ImageResourceLoadEnd && !SDK::ThreadTool.CheckRunning(ImageResourceLoadHandle)) {
			SDK::ThreadTool.Close(ImageResourceLoadHandle);
			std::cout << "Image resource load completed." << std::endl;
			ImageResourceLoadEnd = true;
		}

		if (!SoundResourceLoadEnd && !SDK::ThreadTool.CheckRunning(SoundResourceLoadHandle)) {
			SDK::ThreadTool.Close(SoundResourceLoadHandle);
			std::cout << "Sound resource load completed." << std::endl;
			SoundResourceLoadEnd = true;
		}

		if (!DataResourceLoadEnd && !SDK::ThreadTool.CheckRunning(DataResourceLoadHandle)) {
			SDK::ThreadTool.Close(DataResourceLoadHandle);
			std::cout << "Data resource load completed." << std::endl;
			DataResourceLoadEnd = true;
		}

		if (!FontResourceLoadEnd && !SDK::ThreadTool.CheckRunning(FontResourceLoadHandle)) {
			SDK::ThreadTool.Close(FontResourceLoadHandle);
			std::cout << "Font resource load completed." << std::endl;

			SDK::FontResourceInitializer();
			std::cout << "Font resources created." << std::endl;

			FontResourceLoadEnd = true;
		}

		if (SystemResourceLoadEnd && ImageResourceLoadEnd && SoundResourceLoadEnd && DataResourceLoadEnd && FontResourceLoadEnd)
			return true;

		return false;
	}

	static DWORD WINAPI SystemResourceLoader(LPVOID Param) {
		SDK::ImageTool.LoadImageT(SDK::SYSRES.SDK_LOGO, SDK::SYSRES.SDK_LOGO_IMAGE_DIRECTORY, IMAGE_TYPE_LINEAR);
		SDK::ImageTool.LoadImageT(SDK::SYSRES.FMOD_LOGO, SDK::SYSRES.FMOD_LOGO_DIRECTORY, IMAGE_TYPE_LINEAR);
		SDK::ImageTool.LoadImageT(SDK::SYSRES.COLOR_TEXTURE, SDK::SYSRES.COLOR_TEXTURE_DIRECTORY);
		SDK::SoundTool.Load(SDK::SYSRES.INTRO_SOUND, SDK::SYSRES.SDK_LOGO_SOUND_DIRECTORY);

		SDK::SYSRES.GLU_CIRCLE = gluNewQuadric();
		SDK::SYSRES.GLU_LINE_CIRCLE = gluNewQuadric();
		gluQuadricDrawStyle(SDK::SYSRES.GLU_CIRCLE, GLU_FILL);
		gluQuadricDrawStyle(SDK::SYSRES.GLU_LINE_CIRCLE, GLU_FILL);

		return 0;
	}
};

